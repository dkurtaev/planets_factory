// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_GRASS_FIELD_H_
#define EDITOR_INCLUDE_GRASS_FIELD_H_

#include <vector>

#include "include/structures.h"

class GrassField {
 public:
  GrassField();

  ~GrassField();

  void AddGrassObject(const Triangle* base_triangle);

  void Draw();

  void Init();

 private:
  void SetupTextures();

  void UpdateVBOs();

  unsigned shader_program_;
  unsigned texture_alpha_id_;
  unsigned texture_color_id_;
  std::vector<const Triangle*> base_triangles_;
  bool need_to_update_vbo_;
  unsigned vertices_ids_vbo_;
  unsigned rotations_vbo_;
  unsigned base_tris_normals_vbo_;
  unsigned base_positions_vbo_;
};

#endif  // EDITOR_INCLUDE_GRASS_FIELD_H_
