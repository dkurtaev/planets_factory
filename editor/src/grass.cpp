// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/grass.h"

#include <math.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glog/logging.h>

#include "include/shaders_factory.h"

#define UNIFORM_LOC(name) \
  glGetUniformLocation(shader_program_, name)

#define ATTRIB_LOC(name) \
  glGetAttribLocation(shader_program_, name)

Grass::Grass(const Triangle* base_triangle)
  : GameObject(base_triangle) {
  shader_program_ = ShadersFactory::GetProgramFromFile(
                        "../res/shaders/grass_shader.vertex",
                        "../res/shaders/grass_shader.fragment");
  SetupTextures();
  SetupMesh();
}

Grass::~Grass() {
  delete[] vertices_;
  delete[] tex_coords_;
  delete[] rotations_;
}

void Grass::SetupTextures() {
  cv::Mat texture = cv::imread("./grass_color.png");
  CHECK(texture.data);
  glGenTextures(1, &texture_color_id_);
  glBindTexture(GL_TEXTURE_2D, texture_color_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.cols, texture.rows, 0, GL_RGB,
               GL_UNSIGNED_BYTE, texture.data);

  texture = cv::imread("./grass_alpha.png");
  CHECK(texture.data);
  glGenTextures(1, &texture_alpha_id_);
  glBindTexture(GL_TEXTURE_2D, texture_alpha_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.cols, texture.rows, 0, GL_RGB,
               GL_UNSIGNED_BYTE, texture.data);
}

void Grass::SetupMesh() {
  static const float kWidth = 0.7f;
  static const float kHeight = 0.5f;

  vertices_ = new float[36];
  tex_coords_ = new uint8_t[24];
  rotations_ = new float[12];

  float x = kWidth / 2.0f;
  float z = 0.0f;
  float src_verts[] = {x, 0.0f, z, x, kHeight, z, -x, kHeight, -z, -x, 0.0f, z};
  uint8_t src_tex_coords[] = {255, 255, 255, 0, 0, 0, 0, 255};
  for (uint8_t i = 0; i < 3; ++i) {
    memcpy(vertices_ + i * 12, src_verts, sizeof(float) * 12);
    memcpy(tex_coords_ + i * 8, src_tex_coords, sizeof(uint8_t) * 8);
    for (uint8_t j = 0; j < 4; ++j) {
      rotations_[i * 4 + j] = 60.0f * i;
    }
  }
}

void Grass::Draw() {
  glUseProgram(shader_program_);
  const uint8_t loc_parent_modelview = UNIFORM_LOC("u_planet_modelview_matrix");
  const uint8_t loc_proj_matrix = UNIFORM_LOC("u_projection_matrix");
  const uint8_t loc_tex_alpha = UNIFORM_LOC("u_tex_alpha");
  const uint8_t loc_tex_color = UNIFORM_LOC("u_tex_color");
  const uint8_t loc_self_rotation = ATTRIB_LOC("a_self_rotation");
  const uint8_t loc_base_tri_normal = ATTRIB_LOC("a_base_triangle_normal");
  const uint8_t loc_base_position = ATTRIB_LOC("a_base_position");

  float modelview_matrix[16];
  float projection_matrix[16];
  int8_t base_triangle_normal[36];
  float base_position[36];
  float base_triangle_vertices[9];

  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
  glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);
  base_triangle_->GetNormal(base_triangle_normal);
  base_triangle_->GetCoords(base_triangle_vertices);
  for (uint8_t i = 0; i < 3; ++i) {
    base_position[i] = 0.0f;
    for (uint8_t j = 0; j < 3; ++j) {
      base_position[i] += 0.33f * base_triangle_vertices[j * 3 + i];
    }
  }
  for (uint8_t i = 1; i < 12; ++i) {
    memcpy(base_triangle_normal + i * 3, base_triangle_normal,
           sizeof(int8_t) * 3);
    memcpy(base_position + i * 3, base_position, sizeof(float) * 3);
  }

  glUniformMatrix4fv(loc_parent_modelview, 1, false, modelview_matrix);
  glUniformMatrix4fv(loc_proj_matrix, 1, false, projection_matrix);

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

  unsigned vbo[5];
  glGenBuffers(5, vbo);

  // Coordinates VBOs.
  CHECK_NE(vbo[0], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, vertices_, GL_STATIC_DRAW);
  glVertexAttribPointer(COORDS_ATTRIB, 3, GL_FLOAT, true, 0, 0);
  glEnableVertexAttribArray(COORDS_ATTRIB);

  // Texture coordinates VBO.
  CHECK_NE(vbo[1], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uint8_t) * 24, tex_coords_,
               GL_STATIC_DRAW);
  glVertexAttribPointer(TEX_ATTRIB, 2, GL_UNSIGNED_BYTE, true, 0, 0);
  glEnableVertexAttribArray(TEX_ATTRIB);

  // Self rotations.
  CHECK_NE(vbo[2], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, rotations_,
               GL_STATIC_DRAW);
  glVertexAttribPointer(loc_self_rotation, 1, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(loc_self_rotation);

  // Base triangle normal.
  CHECK_NE(vbo[3], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int8_t) * 36, base_triangle_normal,
               GL_STATIC_DRAW);
  glVertexAttribPointer(loc_base_tri_normal, 3, GL_BYTE, true, 0, 0);
  glEnableVertexAttribArray(loc_base_tri_normal);

  // Base position.
  CHECK_NE(vbo[4], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, base_position,
               GL_STATIC_DRAW);
  glVertexAttribPointer(loc_base_position, 3, GL_FLOAT, true, 0, 0);
  glEnableVertexAttribArray(loc_base_position);

  glDrawArrays(GL_QUADS, 0, 12);

  glDisableVertexAttribArray(loc_base_position);
  glDisableVertexAttribArray(loc_base_tri_normal);
  glDisableVertexAttribArray(loc_self_rotation);
  glDisableVertexAttribArray(TEX_ATTRIB);
  glDisableVertexAttribArray(COORDS_ATTRIB);
  glDeleteBuffers(2, vbo);

  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_CULL_FACE);
  glUseProgram(0);
}
