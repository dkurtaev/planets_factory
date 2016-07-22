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

  void AddGrassObject(const Triangle* base_triangle, float bary_p1,
                      float bary_p2, float bary_p3);

  void Draw(bool sun_shading);

  void Init();

  void Update();

 private:
  void SetupTextures();

  void UpdateVBOs();

  struct Base {
    const Triangle* triangle;
    float bary_coords[3];
  };

  std::vector<Base> bases_;
  unsigned shader_program_;
  unsigned texture_alpha_id_;
  unsigned texture_color_id_;
  bool need_to_update_vbo_;
  unsigned vertices_ids_vbo_;
  unsigned rotations_vbo_;
  unsigned base_tris_normals_vbo_;
  unsigned base_positions_vbo_;
};

#endif  // EDITOR_INCLUDE_GRASS_FIELD_H_
