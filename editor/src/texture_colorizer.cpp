// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/texture_colorizer.h"

#include <vector>
#include <fstream>
#include <string>

#include <glog/logging.h>

const float TextureColorizer::kZeroLimit;

TextureColorizer::TextureColorizer(cv::Mat* texture,
                                   std::vector<Triangle*>* triangles,
                                   const ChangeColorButton* change_color_button,
                                   const BrushSizeButton* brush_size_button,
                                   Switcher* is_enabled_swither,
                                   Backtrace* backtrace)
  : TrianglesToucher(triangles, backtrace), texture_(texture),
    change_color_button_(change_color_button), action_(0),
    brush_size_button_(brush_size_button) {
  static const uint8_t kNumTriangles = 20;
  static const uint8_t kNumTexCoords = 6;
  static const uint8_t kNumTriPoints = 3;
  static const float kNormRatio = 1.0f / UINT16_MAX;

  is_enabled_swither->SetFlag(&is_enabled_);

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
  // Maximal brush size is a half of triangles height.
  // Texture has height of 3 triangles heights.
  const unsigned kMaxBrushSize = texture_->rows * (0.5f / 3.0f);
  const unsigned kBrushSize = brush_size_button_->GetBrushSize(kMaxBrushSize);
  static const uint8_t kNumTriangles = 20;
  static const uint8_t kNumTexCoords = 6;

  CHECK_LE(abs(1.0f - bary_p1 - bary_p2 - bary_p3), kZeroLimit);

  // Setup selected color from palette view.
  uint8_t color[3];
  change_color_button_->GetSelectedColor(color);
  cv::Scalar cv_color(color[2], color[1], color[0]);  // BGR.

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
  if (tex_tri_idx == kNumTriangles) {
    return;
  }

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
  const unsigned mask_height = texture_->rows * rows_ratio;
  const unsigned mask_width = texture_->cols * cols_ratio;

  // Indices of fan center respectively triangles points.
  uint8_t fan_centers[5];
  // Indices of triangles in fan.
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

  // Drawing circle on each texture triangles. For it warp circle center from
  // support mask coordinates to origin texture coordinates.
  float fan_points_tex_coords[][2] = {
    {0.50f, 0.50f}, {0.25f, 0.00f}, {0.00f, 0.50f}, {0.25f, 1.00f},
    {0.75f, 1.00f}, {1.00f, 0.50f}, {0.75f, 0.00f}};

  cv::Point2f cirle_center(0, 0);
  float barycentric_coords[] = {bary_p1, bary_p2, bary_p3};
  uint8_t vert_idx = fan_centers[2];

  cirle_center.x = fan_points_tex_coords[0][0] * barycentric_coords[vert_idx];
  cirle_center.y = fan_points_tex_coords[0][1] * barycentric_coords[vert_idx];
  vert_idx = (vert_idx + 1) % 3;

  for (uint8_t i = 1; i < 3; ++i) {
    cirle_center.x += fan_points_tex_coords[2 + i][0] *
                      barycentric_coords[vert_idx];
    cirle_center.y += fan_points_tex_coords[2 + i][1] *
                      barycentric_coords[vert_idx];
    vert_idx = (vert_idx + 1) % 3;
  }
  cirle_center.x *= mask_width;
  cirle_center.y *= mask_height;

  // Warp each triangle from mask to origin.
  std::vector<cv::Point2f> origin_tex_coords(3);
  std::vector<cv::Point2f> mask_tex_coords(3);
  for (uint8_t i = 0; i < 5; ++i) {
    mask_tex_coords[0].x = fan_points_tex_coords[0][0] * (mask_width - 1);
    mask_tex_coords[0].y = fan_points_tex_coords[0][1] * (mask_height - 1);
    mask_tex_coords[1].x = fan_points_tex_coords[i + 1][0] * (mask_width - 1);
    mask_tex_coords[1].y = fan_points_tex_coords[i + 1][1] * (mask_height - 1);
    mask_tex_coords[2].x = fan_points_tex_coords[i + 2][0] * (mask_width - 1);
    mask_tex_coords[2].y = fan_points_tex_coords[i + 2][1] * (mask_height - 1);

    // For checking that triangle has masked points, check that circle center
    // is inside radius-area of target triangle.
    //  __________________   Where r - brush size (radius).
    //  \   ____|r____   /   0--------> x  Let p1, p2 - points of edge in
    //   \  \        /  /    |    * p2     corresponding order. We need to move
    //    \  \      /  /     |   /         this edge by r in right direction
    //     \r/\    /\r/      |  * p1       (forward direction is p1->p2).
    //      \  \  /  /     y v  --> move   0 -1 - rotation matrix
    //       \  \/  /                      1  0
    //        \    /
    //         \  /                       /|\       => x = 2*r
    //          \/                       / | \
    // \                                / x|  \
    //   \                         90gr/\  |  /\90gr
    //     \         ^                 \/  |  \/
    //       \ 90gr /                    \_|_/  r
    //         \  /                    60gr 60gr
    // ______60gr\
    //       90gr| - - - >
    //           |
    //           v
    cv::Point2f edges_directions[3];
    for (uint8_t j = 0; j < 3; ++j) {
      uint8_t to = (j + 1) % 3;
      edges_directions[j].x = mask_tex_coords[j].y - mask_tex_coords[to].y;
      edges_directions[j].y = mask_tex_coords[to].x - mask_tex_coords[j].x;
      float norm = sqrt(edges_directions[j].x * edges_directions[j].x +
                        edges_directions[j].y * edges_directions[j].y);
      edges_directions[j] *= static_cast<float>(kBrushSize) / norm;
    }
    cv::Point2f augm_mask_tex_coords[3];
    for (uint8_t j = 0; j < 3; ++j) {
      cv::Point2f direction = (edges_directions[j] +
                               edges_directions[(j + 2) % 3]);
      float norm = sqrt(direction.x * direction.x + direction.y * direction.y);
      direction *= static_cast<float>(2 * kBrushSize) / norm;
      augm_mask_tex_coords[j] = mask_tex_coords[j] + direction;
    }
    // Check that circle point at left side of each edge.
    //   0-------> x       a, b - edge points
    //   |   * c           c = circle point
    //   |  /              c at left side of edge (a, b) if determinant
    //   | *----* b        | (b-a).x (b-a).y | < 0.
    // y v  a              | (c-a).x (c-a).y |
    bool has_masked_points = true;
    if (i != 2) {
      for (uint8_t j = 0; j < 3; ++j) {
        uint8_t to = (j + 1) % 3;
        cv::Point2f edge = augm_mask_tex_coords[to] - augm_mask_tex_coords[j];
        cv::Point2f point = cirle_center - augm_mask_tex_coords[j];
        if (edge.x * point.y - edge.y * point.x > kZeroLimit) {
          has_masked_points = false;
          break;
        }
      }
    }

    if (has_masked_points) {
      vert_idx = fan_centers[i];
      float* src = ico_tex_coords_[fan_triangles[i]];
      for (uint8_t j = 0; j < 3; ++j) {
        origin_tex_coords[j].x = src[vert_idx * 2] * (texture_->cols - 1);
        origin_tex_coords[j].y = src[vert_idx * 2 + 1] * (texture_->rows - 1);
        vert_idx = (vert_idx + 1) % 3;
      }

      cv::Rect origin_rect = cv::boundingRect(origin_tex_coords);
      cv::Mat origin_roi = (*texture_)(origin_rect);
      for (uint8_t j = 0; j < 3; ++j) {
        origin_tex_coords[j].x -= origin_rect.tl().x;
        origin_tex_coords[j].y -= origin_rect.tl().y;
      }

      // Compute transformation matrix and warp circle center.
      cv::Mat transf_mat = cv::getAffineTransform(mask_tex_coords,
                                                  origin_tex_coords);
      std::vector<cv::Point2f> warped_points(1, cirle_center);
      cv::transform(warped_points, warped_points, transf_mat);

      cv::circle(origin_roi, warped_points[0], kBrushSize, cv_color, CV_FILLED);
    }
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

void TextureColorizer::InitAction() {
  action_ = new TextureColorizerAction(texture_);
}

void TextureColorizer::FlushAction(Backtrace* backtrace) {
  backtrace->AddAction(action_);
  action_ = 0;
}

float TextureColorizer::GetHighlightingAngle() const {
  // See https://github.com/dkurtaev/planets_factory/issues/1 icosahedron
  // visualization.
  const unsigned kMaxBrushSize = texture_->rows * (0.5f / 3.0f);
  const float kBrushSize = brush_size_button_->GetBrushSize(kMaxBrushSize);
  return (kBrushSize / kMaxBrushSize) * (M_PI / 6.0f);
}

void TextureColorizer::ExportTexture(const std::string& path) const {
  static const unsigned kNumTriangles = 20;

  // Fill black area outside icosahedron triangles.
  cv::Mat texture_copy = texture_->clone();
  cv::Mat mask = cv::Mat::ones(texture_->size(), CV_8UC1);
  std::vector<cv::Point> points(3);

  for (unsigned i = 0; i < kNumTriangles; ++i) {
    float* data = ico_tex_coords_[i];
    for (uint8_t j = 0; j < 3; ++j) {
      points[j].x = data[j * 2] * (texture_->cols - 1);
      points[j].y = data[j * 2 + 1] * (texture_->rows - 1);
    }
    cv::fillConvexPoly(mask, points, cv::Scalar(0));
  }
  cv::erode(mask, mask, cv::Mat());
  texture_copy.setTo(0, mask);
  cv::imwrite(path, texture_copy);
}

void TextureColorizer::ImportTexture(const std::string& path) {
  cv::Mat import_texture = cv::imread(path);
  import_texture.copyTo(*texture_);
}

TextureColorizerAction::TextureColorizerAction(cv::Mat* texture)
  : texture_(texture) {
  texture_copy_ = texture->clone();
}

void TextureColorizerAction::Undo() {
  texture_copy_.copyTo(*texture_);
}
