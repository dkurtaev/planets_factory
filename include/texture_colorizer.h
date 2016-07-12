// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_TEXTURE_COLORIZER_H_
#define INCLUDE_TEXTURE_COLORIZER_H_

#include <vector>
#include <utility>

#include "include/triangles_toucher.h"
#include "include/structures.h"
#include "include/change_color_button.h"

#include <opencv2/opencv.hpp>

class TextureColorizer : public TrianglesToucher {
 public:
  TextureColorizer(cv::Mat* texture,
                   std::vector<Triangle*>* triangles,
                   const ChangeColorButton* change_color_button);

  ~TextureColorizer();

 private:
  static const float kZeroLimit = 1e-4f;

  virtual void DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                        float bary_p3);

  // Returns true if texture triangle with coordinates [triangle_uvs] includes
  // point on texture with coordinates [point_uvs].
  // If true, returns barycentric coordinates of point inside tringle:
  // point_uvs = b1*triangle_uvs[0] + b2*triangle_uvs[1] + b3*triangle_uvs[2],
  // where b3 = 1-b1-b2.
  bool IsIncludes(const float* triangle_uvs, const float* point_uvs,
                  float* bary_p1, float* bary_p2, float* bary_p3) const;

  cv::Mat* texture_;
  const ChangeColorButton* change_color_button_;
  // For each triangle neighbor triangles starts from first two points edge.
  //  ___p1___
  //  \ 1/\ 3/
  //   \/__\/
  // p2 \ 2/ p3
  //     \/
  std::vector<uint8_t*> ico_neighbors_;
  std::vector<float*> ico_tex_coords_;
};

#endif  // INCLUDE_TEXTURE_COLORIZER_H_
