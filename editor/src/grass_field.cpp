// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/grass_field.h"

#include <math.h>

#include <string>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <glog/logging.h>
#include <opencv2/opencv.hpp>

#include "include/shaders_factory.h"

#define UNIFORM_LOC(name) \
  glGetUniformLocation(shader_program_, name)

#define ATTRIB_LOC(name) \
  glGetAttribLocation(shader_program_, name)

GrassField::GrassField()
  : need_to_update_vbo_(false), vertices_ids_vbo_(0), rotations_vbo_(0),
    base_tris_normals_vbo_(0), base_positions_vbo_(0) {}

void GrassField::AddGrassObject(const Triangle* base_triangle, float bary_p1,
                                float bary_p2, float bary_p3) {
  Base base;
  base.triangle = base_triangle;
  base.bary_coords[0] = bary_p1;
  base.bary_coords[1] = bary_p2;
  base.bary_coords[2] = bary_p3;
  bases_.push_back(base);
  need_to_update_vbo_ = true;
}

GrassField::~GrassField() {
  glDeleteBuffers(1, &vertices_ids_vbo_);
  glDeleteBuffers(1, &rotations_vbo_);
  glDeleteBuffers(1, &base_tris_normals_vbo_);
  glDeleteBuffers(1, &base_positions_vbo_);
}

void GrassField::Init() {
  std::vector<std::string> vertex_shaders(1);
  std::vector<std::string> fragment_shaders(2);
  vertex_shaders[0] = "../res/shaders/grass_shader.vertex";
  fragment_shaders[0] = "../res/shaders/grass_shader.fragment";
  fragment_shaders[1] = "../res/shaders/sun_shading.fragment";
  shader_program_ = ShadersFactory::GetProgramFromFile(vertex_shaders,
                                                       fragment_shaders);
  SetupTextures();
}

void GrassField::SetupTextures() {
  cv::Mat texture = cv::imread("./grass_color.png");
  CHECK(texture.data);
  glGenTextures(1, &texture_color_id_);
  glBindTexture(GL_TEXTURE_2D, texture_color_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, texture.cols, texture.rows, 0, GL_BGR,
               GL_UNSIGNED_BYTE, texture.data);

  texture = cv::imread("./grass_alpha.png");
  CHECK(texture.data);
  glGenTextures(1, &texture_alpha_id_);
  glBindTexture(GL_TEXTURE_2D, texture_alpha_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, texture.cols, texture.rows, 0, GL_BGR,
               GL_UNSIGNED_BYTE, texture.data);
}

void GrassField::UpdateVBOs() {
  const unsigned n_grass_objects = bases_.size();
  const unsigned n_quads = 3 * n_grass_objects;
  uint8_t* vertices_ids = new uint8_t[12 * n_grass_objects];
  float* rotations = new float[12 * n_grass_objects];
  int8_t* base_triangle_normal = new int8_t[36 * n_grass_objects];
  float* base_position = new float[36 * n_grass_objects];

  for (uint8_t i = 0; i < 4; ++i) {
    vertices_ids[i] = i;
    for (uint8_t j = 0; j < 3; ++j) {
      rotations[j * 4 + i] = 60.0f * j;
    }
  }
  for (unsigned i = 1; i < 3; ++i) {
    memcpy(vertices_ids + i * 4, vertices_ids, sizeof(uint8_t) * 4);
  }

  float base_triangle_vertices[9];
  float middle_point[3];
  for (unsigned i = 0; i < n_grass_objects; ++i) {
    if (i != 0) {
      memcpy(rotations + i * 12, rotations, sizeof(float) * 12);
      memcpy(vertices_ids + i * 12, vertices_ids, sizeof(uint8_t) * 12);
    }

    const Triangle* triangle = bases_[i].triangle;
    float* bary_coords = bases_[i].bary_coords;
    triangle->GetCoords(base_triangle_vertices);
    triangle->GetNormal(base_triangle_normal + i * 36);

    // Compute middle point of triangle.
    for (uint8_t j = 0; j < 3; ++j) {
      middle_point[j] = 0.0f;
      for (uint8_t k = 0; k < 3; ++k) {
        middle_point[j] += bary_coords[k] * base_triangle_vertices[k * 3 + j];
      }
    }
    memcpy(base_position + i * 36, middle_point, sizeof(float) * 3);

    for (uint8_t j = 1; j < 12; ++j) {
      memcpy(base_triangle_normal + i * 36 + j * 3,
             base_triangle_normal + i * 36, sizeof(int8_t) * 3);
      memcpy(base_position + i * 36 + j * 3,
             base_position + i * 36, sizeof(float) * 3);
    }
  }

  // Vertex kinds VBO.
  glDeleteBuffers(1, &vertices_ids_vbo_);
  glGenBuffers(1, &vertices_ids_vbo_);
  CHECK_NE(vertices_ids_vbo_, 0);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_ids_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uint8_t) * 12 * n_grass_objects,
               vertices_ids, GL_STATIC_DRAW);

  // Self rotations.
  glDeleteBuffers(1, &rotations_vbo_);
  glGenBuffers(1, &rotations_vbo_);
  CHECK_NE(rotations_vbo_, 0);
  glBindBuffer(GL_ARRAY_BUFFER, rotations_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * n_grass_objects,
               rotations, GL_STATIC_DRAW);

  // Base triangle normal.
  glDeleteBuffers(1, &base_tris_normals_vbo_);
  glGenBuffers(1, &base_tris_normals_vbo_);
  CHECK_NE(base_tris_normals_vbo_, 0);
  glBindBuffer(GL_ARRAY_BUFFER, base_tris_normals_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int8_t) * 36 * n_grass_objects,
               base_triangle_normal, GL_STATIC_DRAW);

  // Base position.
  glDeleteBuffers(1, &base_positions_vbo_);
  glGenBuffers(1, &base_positions_vbo_);
  CHECK_NE(base_positions_vbo_, 0);
  glBindBuffer(GL_ARRAY_BUFFER, base_positions_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * n_grass_objects,
               base_position, GL_STATIC_DRAW);

  need_to_update_vbo_ = false;
  delete[] vertices_ids;
  delete[] rotations;
  delete[] base_triangle_normal;
  delete[] base_position;
}

void GrassField::Draw(bool sun_shading) {
  static const float kWidth = 0.7f;
  static const float kHeight = 0.5f;

  if (bases_.empty()) {
    return;
  }

  if (need_to_update_vbo_) {
    UpdateVBOs();
  }

  glUseProgram(shader_program_);
  const uint8_t loc_parent_modelview = UNIFORM_LOC("u_planet_modelview_matrix");
  const uint8_t loc_proj_matrix = UNIFORM_LOC("u_projection_matrix");
  const uint8_t loc_tex_alpha = UNIFORM_LOC("u_tex_alpha");
  const uint8_t loc_tex_color = UNIFORM_LOC("u_tex_color");
  const uint8_t loc_grass_size = UNIFORM_LOC("u_grass_size");
  const uint8_t loc_self_rotation = ATTRIB_LOC("a_self_rotation");
  const uint8_t loc_base_tri_normal = ATTRIB_LOC("a_base_triangle_normal");
  const uint8_t loc_base_position = ATTRIB_LOC("a_base_position");
  const uint8_t loc_point_idx = ATTRIB_LOC("a_point_idx");

  //
  const uint8_t loc_use_sun_shading = UNIFORM_LOC("u_sun_shading");
  const uint8_t loc_sun_position = UNIFORM_LOC("u_sun_position");
  const uint8_t loc_sun_radius = UNIFORM_LOC("u_sun_radius");
  const uint8_t loc_planet_position = UNIFORM_LOC("u_planet_position");
  const uint8_t loc_planet_radius = UNIFORM_LOC("u_planet_radius");

  glUniform1i(loc_use_sun_shading, sun_shading);
  glUniform3f(loc_sun_position, 100.0f, 100.0f, 100.0f);
  glUniform1f(loc_sun_radius, 10.0f);
  glUniform3f(loc_planet_position, 0.0f, 0.0f, 0.0f);
  glUniform1f(loc_planet_radius, 4.0f);
  //

  float modelview_matrix[16];
  float projection_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
  glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);

  glUniformMatrix4fv(loc_parent_modelview, 1, false, modelview_matrix);
  glUniformMatrix4fv(loc_proj_matrix, 1, false, projection_matrix);
  glUniform2f(loc_grass_size, kWidth, kHeight);

  glUniform1i(loc_tex_alpha, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_alpha_id_);

  glUniform1i(loc_tex_color, 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture_color_id_);

  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);

  // Vertex kinds VBO.
  glBindBuffer(GL_ARRAY_BUFFER, vertices_ids_vbo_);
  glVertexAttribPointer(loc_point_idx, 1, GL_UNSIGNED_BYTE, false, 0, 0);
  glEnableVertexAttribArray(loc_point_idx);

  // Self rotations.
  glBindBuffer(GL_ARRAY_BUFFER, rotations_vbo_);
  glVertexAttribPointer(loc_self_rotation, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(loc_self_rotation);

  // Base triangle normal.
  glBindBuffer(GL_ARRAY_BUFFER, base_tris_normals_vbo_);
  glVertexAttribPointer(loc_base_tri_normal, 3, GL_BYTE, true, 0, 0);
  glEnableVertexAttribArray(loc_base_tri_normal);

  // Base position.
  glBindBuffer(GL_ARRAY_BUFFER, base_positions_vbo_);
  glVertexAttribPointer(loc_base_position, 3, GL_FLOAT, true, 0, 0);
  glEnableVertexAttribArray(loc_base_position);

  glDrawArrays(GL_QUADS, 0, 12 * bases_.size());

  glDisableVertexAttribArray(loc_point_idx);
  glDisableVertexAttribArray(loc_base_position);
  glDisableVertexAttribArray(loc_base_tri_normal);
  glDisableVertexAttribArray(loc_self_rotation);

  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_CULL_FACE);
  glUseProgram(0);
}

void GrassField::Update() {
  need_to_update_vbo_ = true;
}

void GrassField::RemoveLastGrassObjects(unsigned n_objects) {
  need_to_update_vbo_ = true;
  CHECK_GE(bases_.size(), n_objects);
  bases_.erase(bases_.end() - n_objects, bases_.end());
}
