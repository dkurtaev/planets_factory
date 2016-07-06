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

 private:
  virtual void DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                        float bary_p3);

  cv::Mat* texture_;
  const ChangeColorButton* change_color_button_;
};

#endif  // INCLUDE_TEXTURE_COLORIZER_H_
