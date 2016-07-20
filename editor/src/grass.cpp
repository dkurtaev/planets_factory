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

  float x = kWidth / 2.0f;
  float z = 0.0f;
  float r_matrix[4];
  r_matrix[0] = r_matrix[3] = 0.5f;
  r_matrix[1] = sqrt(3.0f) / 2.0f;
  r_matrix[2] = -r_matrix[1];

  uint8_t single_tex_coords[] = {255, 255, 255, 0, 0, 0, 0, 255};
  float* dst = vertices_;
  for (uint8_t i = 0; i < 3; ++i) {
    dst[0] = x; dst[1] = 0.0f; dst[2] = z;
    dst[3] = x; dst[4] = kHeight; dst[5] = z;
    dst[6] = -x; dst[7] = kHeight; dst[8] = -z;
    dst[9] = -x; dst[10] = 0.0f; dst[11] = -z;

    x = r_matrix[0] * dst[0] + r_matrix[1] * dst[2];
    z = r_matrix[2] * dst[0] + r_matrix[3] * dst[2];

    dst += 12;
    memcpy(tex_coords_ + i * 8, single_tex_coords, sizeof(uint8_t) * 8);
  }
}

void Grass::DrawObject() {
  glUseProgram(shader_program_);
  const uint8_t loc_model_matrix = UNIFORM_LOC("u_modelview_matrix");
  const uint8_t loc_proj_matrix = UNIFORM_LOC("u_projection_matrix");
  const uint8_t loc_tex_alpha = UNIFORM_LOC("u_tex_alpha");
  const uint8_t loc_tex_color = UNIFORM_LOC("u_tex_color");

  float modelview_matrix[16];
  float projection_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
  glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);

  glUniformMatrix4fv(loc_model_matrix, 1, false, modelview_matrix);
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

  unsigned vbo[2];
  glGenBuffers(2, vbo);

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

  glDrawArrays(GL_QUADS, 0, 12);

  glDisableVertexAttribArray(TEX_ATTRIB);
  glDisableVertexAttribArray(COORDS_ATTRIB);
  glDeleteBuffers(2, vbo);

  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_CULL_FACE);
  glUseProgram(0);
}
