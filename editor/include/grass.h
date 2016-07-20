// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_GRASS_FIELD_H_
#define EDITOR_INCLUDE_GRASS_FIELD_H_

#include <vector>

#include "include/structures.h"

class GrassField {
 public:
  void AddGrassObject(const Triangle* base_triangle);

  void Draw() const;

  void Init();

 private:
  void SetupTextures();

  unsigned shader_program_;
  unsigned texture_alpha_id_;
  unsigned texture_color_id_;
  std::vector<const Triangle*> base_triangles_;
};

#endif  // EDITOR_INCLUDE_GRASS_FIELD_H_
