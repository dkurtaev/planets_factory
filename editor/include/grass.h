// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_GRASS_H_
#define EDITOR_INCLUDE_GRASS_H_

#include <opencv2/opencv.hpp>

#include "include/game_object.h"

class Grass : public GameObject {
 public:
  Grass(const Triangle* base_triangle);

  ~Grass();

  virtual void Draw();

 private:
  void SetupTextures();

  void SetupMesh();

  unsigned shader_program_;
  unsigned texture_alpha_id_;
  unsigned texture_color_id_;
  float* vertices_;
  uint8_t* tex_coords_;
};

#endif  // EDITOR_INCLUDE_GRASS_H_
