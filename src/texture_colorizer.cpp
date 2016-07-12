// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/texture_colorizer.h"

#include <vector>
#include <utility>
#include <fstream>
#include <string>

#include <glog/logging.h>

const float TextureColorizer::kZeroLimit;

TextureColorizer::TextureColorizer(cv::Mat* texture,
                                   std::vector<Triangle*>* triangles,
                                   const ChangeColorButton* change_color_button,
                                   Switcher* is_enabled_swither)
  : TrianglesToucher(triangles), change_color_button_(change_color_button),
    texture_(texture) {
  is_enabled_swither->SetFlag(&is_enabled_);

  static const uint8_t kNumTriangles = 20;
  static const uint8_t kNumTexCoords = 6;
  static const uint8_t kNumTriPoints = 3;
  static const float kNormRatio = 1.0f / UINT16_MAX;

  // Read texture data from resource files.
  std::ifstream tex_coords_file("../res/tex_coords/coords_1.txt");
  CHECK(tex_coords_file.is_open());

  ico_tex_coords_.resize(kNumTriangles);
  for (uint8_t i = 0; i < kNumTriangles; ++i) {
    float* new_tex_coords = new float[kNumTexCoords];
    for (uint8_t j = 0; j < kNumTexCoords; ++j) {
      tex_coords_file >> new_tex_coords[j];
      new_tex_coords[j] *= kNormRatio;
    }
    ico_tex_coords_[i] = new_tex_coords;
  }

  int tmp;
  ico_neighbors_.resize(kNumTriangles);
  for (uint8_t i = 0; i < kNumTriangles; ++i) {
    uint8_t* neighbors = new uint8_t[3];
    for (uint8_t j = 0; j < 3; ++j) {
      tex_coords_file >> tmp;
      neighbors[j] = tmp;
    }
    ico_neighbors_[i] = neighbors;
  }
  tex_coords_file.close();
}

TextureColorizer::~TextureColorizer() {
  unsigned size = ico_neighbors_.size();
  CHECK_EQ(ico_tex_coords_.size(), size);
  for (unsigned i = 0; i < size; ++i) {
    delete[] ico_neighbors_[i];
    delete[] ico_tex_coords_[i];
  }
}

void TextureColorizer::DoAction(Triangle* triangle, float bary_p1,
                                float bary_p2, float bary_p3) {
  static const unsigned kCircleRadius = 10;
  static const uint8_t kNumTriangles = 20;
  static const uint8_t kNumTexCoords = 6;

  CHECK_LE(abs(1.0f - bary_p1 - bary_p2 - bary_p3), kZeroLimit);

  // Setup selected color from palette view.
  uint8_t color[3];
  change_color_button_->GetSelectedColor(color);
  cv::Scalar cv_color(color[0], color[1], color[2]);

  // Compute texture coordinates of touch point.
  float tex_coords[kNumTexCoords];
  triangle->GetTexCoords(tex_coords);
  float touch_uv[] = {bary_p1 * tex_coords[0] + bary_p2 * tex_coords[2] +
                      bary_p3 * tex_coords[4],
                      bary_p1 * tex_coords[1] + bary_p2 * tex_coords[3] +
                      bary_p3 * tex_coords[5]};

  // Find texture triangle includes touch point.
  uint8_t tex_tri_idx;
  for (tex_tri_idx = 0; tex_tri_idx < kNumTriangles; ++tex_tri_idx) {
    if (IsIncludes(ico_tex_coords_[tex_tri_idx], touch_uv,
                   &bary_p1, &bary_p2, &bary_p3)) {
      break;
    }
  }
  CHECK_LT(tex_tri_idx, kNumTriangles);

  // Create support mask:
  //     0.00  0.25  0.50  0.75  1.00
  //      +------------------------+   width: 2/5.5 of texture width
  // 0.00 |*****/\**********/\*****|   height: 2/3 of texture height
  //      |****/01\********/01\****|
  //      |***/    \******/    \***|
  //      |**/ [00] \****/ [04] \**|
  //      |*/        \**/        \*|
  // 0.50 |/02 ____ 00\/00 ____ 05\|
  //      |\02      00/\00      05/|
  //      |*\        /00\        /*|
  //      |**\ [01] /    \ [03] /**|
  //      |***\    / [02] \    /***|
  //      |****\03/        \04/****|
  // 1.00 |*****\/03 ____ 04\/*****|
  //      +------------------------+
  // Where 00 - nearest vertex to touch point,
  // (00, 03, 04) - found triangle under touch.
  const float rows_ratio = 2.0f / 3.0f;
  const float cols_ratio = 2.0f / 5.5f;
  cv::Mat mask = cv::Mat::zeros(texture_->rows * rows_ratio,
                                texture_->cols * cols_ratio, CV_8UC1);

  // Indices of fan center respectively triangles points.
  uint8_t fan_centers[5];
  // Indices of triangles of fan.
  uint8_t fan_triangles[5];
  fan_centers[2] = (bary_p2 > bary_p1 ? (bary_p3 > bary_p2 ? 2 : 1) :
                                        (bary_p3 > bary_p1 ? 2 : 0));
  fan_triangles[2] = tex_tri_idx;

  for (int8_t i = 1; i >= 0; --i) {
    // ______________________  fc - fan center           v
    //        fc/\fc        /  [i+1] neighbors: [i+2], [i], [?]
    //         /fc\        /   [i+1] points:       p1,  fc,  p2
    // \  [i] /    \ [i+2]/                              ^
    //  \    / [i+1]\    /
    //   \  /        \  /
    // p2 \/__________\/ p1
    //     \    [?]   /
    //      \        /
    fan_triangles[i] = ico_neighbors_[fan_triangles[i + 1]][fan_centers[i + 1]];
    uint8_t* neighbors = ico_neighbors_[fan_triangles[i]];
    // ______________________  fc - fan center    v
    //        fc/\fc        /  [i] neighbors: [i+1], [i-1], [?]
    //         /fc\  [i+1] /   [i+1] points:     p1,    fc,  p2
    // \ [i-1]/    \      /                              ^
    //  \    /  [i] \    /
    //   \  /        \  /
    // p2 \/__________\/ p1
    //     \    [?]   /
    //      \        /
    for (uint8_t j = 0; j < 3; ++j) {
      if (neighbors[j] == fan_triangles[i + 1]) {
        fan_centers[i] = (j + 1) % 3;
        break;
      }
    }
  }

  for (uint8_t i = 3; i <= 4; ++i) {
    // ______________________  fc - fan center                v
    //        fc/\fc        /  [i-1] neighbors: [i-2], [?], [i]
    //         /fc\        /   [i-1] points:       fc,  p2,  p1
    // \ [i-2]/    \  [i] /                         ^
    //  \    / [i-1]\    /
    //   \  /        \  /
    // p2 \/__________\/ p1
    //     \    [?]   /
    //      \        /
    fan_triangles[i] = ico_neighbors_[fan_triangles[i - 1]]
                                     [(fan_centers[i - 1] + 2) % 3];
    uint8_t* neighbors = ico_neighbors_[fan_triangles[i]];
    // ______________________  fc - fan center           v
    //        fc/\fc        /  [i] neighbors: [i+1], [i-1], [?]
    //         /fc\   [i+1]/   [i+1] points:     p1,    fc,  p2
    // \ [i-1]/    \      /                              ^
    //  \    /  [i] \    /
    //   \  /        \  /
    // p2 \/__________\/ p1
    //     \    [?]   /
    //      \        /
    for (uint8_t j = 0; j < 3; ++j) {
      if (neighbors[j] == fan_triangles[i - 1]) {
        fan_centers[i] = j;
        break;
      }
    }
  }

  float origin_tex_coords[5][6];
  for (uint8_t i = 0; i < 5; ++i) {
    uint8_t vert_idx = fan_centers[i];
    float* dst = origin_tex_coords[i];
    float* src = ico_tex_coords_[fan_triangles[i]];
    for (uint8_t j = 0; j < 3; ++j) {
      dst[j * 2] = src[vert_idx * 2] * texture_->cols;
      dst[j * 2 + 1] = src[vert_idx * 2 + 1] * texture_->rows;
      vert_idx = (vert_idx + 1) % 3;
    }
  }

  float fan_points_tex_coords[][2] = {
    {0.50f, 0.50f}, {0.25f, 0.00f}, {0.00f, 0.50f}, {0.25f, 1.00f},
    {0.75f, 1.00f}, {1.00f, 0.50f}, {0.75f, 0.00f}};
  float mask_tex_coords[5][6];
  for (uint8_t i = 0; i < 5; ++i) {
    float* dst = mask_tex_coords[i];
    dst[0] = fan_points_tex_coords[0][0] * mask.cols;
    dst[1] = fan_points_tex_coords[0][1] * mask.rows;
    dst[2] = fan_points_tex_coords[i + 1][0] * mask.cols;
    dst[3] = fan_points_tex_coords[i + 1][1] * mask.rows;
    dst[4] = fan_points_tex_coords[i + 2][0] * mask.cols;
    dst[5] = fan_points_tex_coords[i + 2][1] * mask.rows;
  }

  // Drawing circle on mask.
  cv::Point2f cirle_center(0, 0);
  float barycentric_coords[] = {bary_p1, bary_p2, bary_p3};
  uint8_t vert_idx = fan_centers[2];
  for (uint8_t i = 0; i < 3; ++i) {
    cirle_center.x += mask_tex_coords[2][i * 2] *
                      barycentric_coords[vert_idx];
    cirle_center.y += mask_tex_coords[2][i * 2 + 1] *
                      barycentric_coords[vert_idx];
    vert_idx = (vert_idx + 1) % 3;
  }
  cv::circle(mask, cirle_center, kCircleRadius, cv::Scalar(1), CV_FILLED);

  // Warp to origin texture.
  for (uint8_t i = 0; i < 5; ++i) {
    // Compute transformation matrix.
    cv::Point2f src[3];
    cv::Point src_int[3];
    cv::Point2f dst[3];
    for (uint8_t j = 0; j < 3; ++j) {
      src[j].x = mask_tex_coords[i][j * 2];
      src[j].y = mask_tex_coords[i][j * 2 + 1];
      src_int[j].x = src[j].x;
      src_int[j].y = src[j].y;
      dst[j].x = origin_tex_coords[i][j * 2];
      dst[j].y = origin_tex_coords[i][j * 2 + 1];
    }
    cv::Mat transf_mat = cv::getAffineTransform(src, dst);

    // Mask of triangle of interest.
    cv::Mat submask = cv::Mat::zeros(mask.size(), CV_8UC1);
    cv::fillConvexPoly(submask, src_int, 3, cv::Scalar(1));
    submask &= mask;
    // Apply transformation matrix.
    cv::warpAffine(submask, submask, transf_mat, texture_->size());
    texture_->setTo(cv_color, submask);
  }
}

bool TextureColorizer::IsIncludes(const float* triangle_uvs,
                                  const float* point_uvs, float* bary_p1,
                                  float* bary_p2, float* bary_p3) const {
  // triangle_uvs: p1u, p1v, p2u, p2v, p3u, p3v,
  // point_uvs: u, v
  // barycentric coordinates: b1, b2, b3
  // u = b1*p1u + b2*p2u + (1-b1-b2)*p3u
  // v = b1*p1v + b2*p2v + (1-b1-b2)*p3v

  // u - p3u = b1*(p1u - p3u) + b2*(p2u - p3u)
  // v - p3v = b1*(p1v - p3v) + b2*(p2v - p3v)

  // Triangle includes point if all barycentric coordinates in [0, 1].
  float cols[][2] = {{triangle_uvs[0], triangle_uvs[1]},
                     {triangle_uvs[2], triangle_uvs[3]},
                     {point_uvs[0], point_uvs[1]}};
  for (uint8_t i = 0; i < 3; ++i) {
    cols[i][0] -= triangle_uvs[4];
    cols[i][1] -= triangle_uvs[5];
  }
  float determinant = Determinant(cols[0], cols[1]);
  *bary_p1 = Determinant(cols[2], cols[1]) / determinant;
  if (*bary_p1 < -kZeroLimit || 1.0f + kZeroLimit < *bary_p1) {
    return false;
  }
  *bary_p2 = Determinant(cols[0], cols[2]) / determinant;
  if (*bary_p2 < -kZeroLimit || 1.0f + kZeroLimit < *bary_p2) {
    return false;
  }
  *bary_p3 = 1.0f - *bary_p1 - *bary_p2;
  return (-kZeroLimit <= *bary_p3 && *bary_p3 <= 1.0f + kZeroLimit);
}
