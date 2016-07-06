// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/texture_colorizer.h"

#include <vector>
#include <utility>

TextureColorizer::TextureColorizer(cv::Mat* texture,
                                   std::vector<Triangle*>* triangles,
                                   const ChangeColorButton* change_color_button)
  : TrianglesToucher(triangles), change_color_button_(change_color_button),
    texture_(texture) {}

void TextureColorizer::DoAction(Triangle* triangle, float bary_p1,
                                float bary_p2, float bary_p3) {
  uint8_t color[3];
  change_color_button_->GetSelectedColor(color);

  uint16_t tex_coords[6];
  triangle->GetTexCoords(tex_coords);

  cv::Point cirle_center;
  cirle_center.x = (bary_p1 * tex_coords[0] + bary_p2 * tex_coords[2] +
                   bary_p3 * tex_coords[4]) *
                   static_cast<float>(texture_->cols) / UINT16_MAX;
  cirle_center.y = (bary_p1 * tex_coords[1] + bary_p2 * tex_coords[3] +
                   bary_p3 * tex_coords[5]) *
                   static_cast<float>(texture_->rows) / UINT16_MAX;

  cv::circle(*texture_, cirle_center, 5, cv::Scalar(0, 204, 0), CV_FILLED);
  cv::imwrite("./texture.png", *texture_);

  // const unsigned n_vertices = area->size();
  // for (unsigned i = 0; i < n_vertices; ++i) {
  //   area->operator[](i).second->SetColor(color[0], color[1], color[2]);
  // }
}
