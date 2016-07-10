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
                                   const ChangeColorButton* change_color_button)
  : TrianglesToucher(triangles), change_color_button_(change_color_button),
    texture_(texture) {
  static const uint8_t kNumTriangles = 20;
  static const uint8_t kNumTexCoords = 6;
  static const uint8_t kNumTriPoints = 3;
  static const float kNormRatio = 1.0f / UINT16_MAX;

  // Read texture data from resource files.
  std::string paths[] = {"../res/tex_coords/coords_1.txt",
                         "../res/tex_coords/coords_2.txt"};
  std::vector<float*>* src_coords[] = {&ico_tex_coords_, &alt_tex_coords_};
  std::vector<cv::Point*>* src_points[] = {&ico_tex_points_, &alt_tex_points_};

  for (uint8_t i = 0; i < 2; ++i) {
    std::ifstream tex_coords_file(paths[i].c_str());
    CHECK(tex_coords_file.is_open());

    src_coords[i]->resize(kNumTriangles);
    src_points[i]->resize(kNumTriangles);
    for (uint8_t j = 0; j < kNumTriangles; ++j) {
      float* new_tex_coords = new float[kNumTexCoords];
      for (uint8_t k = 0; k < kNumTexCoords; ++k) {
        tex_coords_file >> new_tex_coords[k];
        new_tex_coords[k] *= kNormRatio;
      }
      src_coords[i]->operator[](j) = new_tex_coords;

      cv::Point* new_tex_points = new cv::Point[kNumTriPoints];
      for (uint8_t k = 0; k < kNumTriPoints; ++k) {
        new_tex_points[k].x = new_tex_coords[k * 2] * texture_->cols;
        new_tex_points[k].y = new_tex_coords[k * 2 + 1] * texture_->rows;
      }
      src_points[i]->operator[](j) = new_tex_points;
    }
    tex_coords_file.close();
  }

  // Compute transformation matrices.
  cv::Point2f src[kNumTriPoints];
  cv::Point2f dst[kNumTriPoints];
  transf_mats_.resize(kNumTriangles);
  for (uint8_t i = 0; i < kNumTriangles; ++i) {
    for (uint8_t j = 0; j < kNumTriPoints; ++j) {
      src[j].x = alt_tex_points_[i][j].x;
      src[j].y = alt_tex_points_[i][j].y;
      dst[j].x = ico_tex_points_[i][j].x;
      dst[j].y = ico_tex_points_[i][j].y;
    }
    transf_mats_[i] = cv::getAffineTransform(src, dst);
  }
}

TextureColorizer::~TextureColorizer() {
  unsigned size = ico_tex_coords_.size();
  CHECK_EQ(size, alt_tex_coords_.size());
  for (unsigned i = 0; i < size; ++i) {
    delete[] ico_tex_coords_[i];
    delete[] alt_tex_coords_[i];
    delete[] ico_tex_points_[i];
    delete[] alt_tex_points_[i];
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

  // // Drawing on origin texture.
  cv::Point2f cirle_center(touch_uv[0] * texture_->cols,
                           touch_uv[1] * texture_->rows);
  cv::circle(*texture_, cirle_center, kCircleRadius, cv_color, CV_FILLED);

  // Find texture triangle includes touch point.
  uint8_t tex_tri_idx;
  for (tex_tri_idx = 0; tex_tri_idx < kNumTriangles; ++tex_tri_idx) {
    if (IsIncludes(ico_tex_coords_[tex_tri_idx], touch_uv)) {
      break;
    }
  }
  CHECK_LT(tex_tri_idx, kNumTriangles);

  // Compute touch point on alternative texture layout.
  float* ico_uvs = ico_tex_coords_[tex_tri_idx];
  float* alt_uvs = alt_tex_coords_[tex_tri_idx];
  ComputeBarycentric(ico_uvs, touch_uv, &bary_p1, &bary_p2, &bary_p3);
  float alt_touch_uv[] = {bary_p1 * alt_uvs[0] + bary_p2 * alt_uvs[2] +
                          bary_p3 * alt_uvs[4],
                          bary_p1 * alt_uvs[1] + bary_p2 * alt_uvs[3] +
                          bary_p3 * alt_uvs[5]};

  // Make mask of same circle but on alternative texture layout.
  cv::Mat circle_mask = cv::Mat::zeros(texture_->size(), CV_8UC1);
  cirle_center.x = alt_touch_uv[0] * texture_->cols;
  cirle_center.y = alt_touch_uv[1] * texture_->rows;
  cv::circle(circle_mask, cirle_center, kCircleRadius, cv::Scalar(1),
             CV_FILLED);

  // Drawing on origin texture by mask warped to corresponding triangle.
  const uint8_t n_pts = 3;
  for (uint8_t i = 0; i < kNumTriangles; ++i) {
    // Mask of triangle of interest.
    cv::Mat submask = cv::Mat::zeros(circle_mask.size(), CV_8UC1);
    cv::fillConvexPoly(submask, alt_tex_points_[i], n_pts, cv::Scalar(1));
    submask &= circle_mask;
    // Apply transformation matrix.
    cv::warpAffine(submask, submask, transf_mats_[i], submask.size());
    texture_->setTo(cv_color, submask);
  }
}

void TextureColorizer::ComputeBarycentric(const float* triangle_uvs,
                                          const float* point_uvs,
                                          float* bary_p1, float* bary_p2,
                                          float* bary_p3) const {
  // triangle_uvs: p1u, p1v, p2u, p2v, p3u, p3v,
  // u = b1*p1u + b2*p2u + (1-b1-b2)*p3u
  // v = b1*p1v + b2*p2v + (1-b1-b2)*p3v

  // u - p3u = b1*(p1u - p3u) + b2*(p2u - p3u)
  // v - p3v = b1*(p1v - p3v) + b2*(p2v - p3v)
  float cols[][2] = {{triangle_uvs[0], triangle_uvs[1]},
                     {triangle_uvs[2], triangle_uvs[3]},
                     {point_uvs[0], point_uvs[1]}};
  for (uint8_t i = 0; i < 3; ++i) {
    cols[i][0] -= triangle_uvs[4];
    cols[i][1] -= triangle_uvs[5];
  }
  float determinant = Determinant(cols[0], cols[1]);
  *bary_p1 = Determinant(cols[2], cols[1]) / determinant;
  *bary_p2 = Determinant(cols[0], cols[2]) / determinant;
  *bary_p3 = 1.0f - *bary_p1 - *bary_p2;
}

bool TextureColorizer::IsIncludes(const float* triangle_uvs,
                                  const float* point_uvs) const {
  // Triangle includes point if point at the left side of all triangle's edges.
  // p1 <_________ p2
  //    \        ^
  //     \   *p /
  //      \    /
  //       \  /
  //        v/ p3
  // Point (x, y) is at left side of vector (a, b) if |a b| < 0
  //                                                  |x y|
  // 0 ------------> x
  // |  from
  // |    *----> p    (a, b) = to - from
  // |     \          (x, y) = p - from
  // |      \
  // v y     v to
  uint8_t from, to;
  float a, b, x, y;
  for (uint8_t i = 0; i < 3; ++i) {
    from = i * 2;
    to = ((i + 1) % 3) * 2;
    a = triangle_uvs[to] - triangle_uvs[from];
    b = triangle_uvs[to + 1] - triangle_uvs[from + 1];
    x = point_uvs[0] - triangle_uvs[from];
    y = point_uvs[1] - triangle_uvs[from + 1];
    if (a * y - x * b > kZeroLimit) {
      return false;
    }
  }
  return true;
}
