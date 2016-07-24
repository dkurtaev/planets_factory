// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/planet_view.h"

#include <string>
#include <vector>

#include <GL/freeglut.h>
#include <glog/logging.h>

#include "include/shaders_factory.h"

#define PLANET_SHADER_LOC(attrib) \
  glGetUniformLocation(planet_shader_program_, attrib)

#define GRID_SHADER_LOC(attrib) \
  glGetUniformLocation(grid_shader_program_, attrib)

PlanetView::PlanetView(Icosphere* icosphere, SphericalCS* camera_cs,
                       const cv::Mat* texture, bool* draw_grid, bool* draw_mesh,
                       bool* sun_shading, TextureColorizer* texture_colorizer,
                       VerticesMover* vertices_mover, GrassField* grass_field,
                       bool* draw_grass)
  : GLView(500, 500, "Planets factory"), icosphere_(icosphere),
    camera_(camera_cs), texture_(texture), draw_grid_(draw_grid),
    draw_mesh_(draw_mesh), texture_colorizer_(texture_colorizer),
    vertices_mover_(vertices_mover), sun_shading_(sun_shading),
    grass_field_(grass_field), draw_grass_(draw_grass) {
  InitGL();

  std::vector<std::string> vertex_shaders(1);
  std::vector<std::string> fragment_shaders(2);
  vertex_shaders[0] = "../res/shaders/planet_shader.vertex";
  fragment_shaders[0] = "../res/shaders/planet_shader.fragment";
  fragment_shaders[1] = "../res/shaders/sun_shading.fragment";
  planet_shader_program_ = ShadersFactory::GetProgramFromFile(vertex_shaders,
                                                              fragment_shaders);
  fragment_shaders.resize(1);
  vertex_shaders[0] = "../res/shaders/icogrid_shader.vertex";
  fragment_shaders[0] = "../res/shaders/icogrid_shader.fragment";
  grid_shader_program_ = ShadersFactory::GetProgramFromFile(vertex_shaders,
                                                            fragment_shaders);
  SetTexture();
  AddIcosphereToucher(&highlighting_toucher_);
}

void PlanetView::Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera_.Setup(display_width_, display_height_);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float modelview_matrix[16];
  float projection_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
  glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);

  if (*draw_mesh_) {
    glUseProgram(planet_shader_program_);
    const uint8_t loc_model_matrix = PLANET_SHADER_LOC("u_modelview_matrix");
    const uint8_t loc_proj_matrix = PLANET_SHADER_LOC("u_projection_matrix");
    const uint8_t loc_touch = PLANET_SHADER_LOC("u_process_touch");
    const uint8_t loc_touch_angle = PLANET_SHADER_LOC("u_touch_angle");
    const uint8_t loc_touch_coord = PLANET_SHADER_LOC("u_touch_coord");
    const uint8_t loc_sun_position = PLANET_SHADER_LOC("u_sun_position");
    const uint8_t loc_sun_radius = PLANET_SHADER_LOC("u_sun_radius");
    const uint8_t loc_planet_position = PLANET_SHADER_LOC("u_planet_position");
    const uint8_t loc_planet_radius = PLANET_SHADER_LOC("u_planet_radius");
    const uint8_t loc_use_sun_shading = PLANET_SHADER_LOC("u_sun_shading");
    const uint8_t loc_texture = PLANET_SHADER_LOC("u_texture");

    glUniformMatrix4fv(loc_model_matrix, 1, false, modelview_matrix);
    glUniformMatrix4fv(loc_proj_matrix, 1, false, projection_matrix);

    glUniform1i(loc_use_sun_shading, *sun_shading_);
    glUniform3f(loc_sun_position, 100.0f, 100.0f, 100.0f);
    glUniform1f(loc_sun_radius, 10.0f);
    glUniform3f(loc_planet_position, 0.0f, 0.0f, 0.0f);
    glUniform1f(loc_planet_radius, 4.0f);

    // Highlighting ring.
    if (highlighting_toucher_.HasTouchPoint() &&
        (texture_colorizer_->IsEnabled() || vertices_mover_->IsEnabled())) {
      float touch_point[3];
      highlighting_toucher_.GetTouchPoint(touch_point);
      glUniform1i(loc_touch, true);
      glUniform3fv(loc_touch_coord, 1, touch_point);
      if (texture_colorizer_->IsEnabled()) {
        glUniform1f(loc_touch_angle,
                    texture_colorizer_->GetHighlightingAngle());
      } else {
        const uint8_t n_splits = icosphere_->GetNumSplits();
        glUniform1f(loc_touch_angle,
                    vertices_mover_->GetHighlightingAngle(n_splits));
      }
    } else {
      glUniform1i(loc_touch, false);
    }

    // Texture.
    glUniform1i(loc_texture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_->cols, texture_->rows,
                 0, GL_BGR, GL_UNSIGNED_BYTE, texture_->data);

    icosphere_->Draw();
    glUseProgram(0);

    const unsigned n_touchers = icosphere_touchers_.size();
    for (unsigned i = 0; i < n_touchers; ++i) {
      icosphere_touchers_[i]->SolveTouchRequests();
    }
  }

  if (*draw_grid_) {
    glUseProgram(grid_shader_program_);
    const uint8_t loc_model_matrix = GRID_SHADER_LOC("u_modelview_matrix");
    const uint8_t loc_proj_matrix = GRID_SHADER_LOC("u_projection_matrix");
    glUniformMatrix4fv(loc_model_matrix, 1, false, modelview_matrix);
    glUniformMatrix4fv(loc_proj_matrix, 1, false, projection_matrix);
    icosphere_->DrawGrid();
    glUseProgram(0);
  }

  if (*draw_grass_) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadMatrixf(modelview_matrix);
      grass_field_->Draw(*sun_shading_);
    glPopMatrix();
  }
  glutSwapBuffers();
}

void PlanetView::InitGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
}

void PlanetView::SetTexture() {
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void PlanetView::AddIcosphereToucher(Toucher* toucher) {
  icosphere_touchers_.push_back(toucher);
  AddListener(toucher);
}

void HighlightingToucher::MouseMove(int x, int y) {
  callback_requests_.push(MOUSE_MOVE);
  requests_data_.push(x);
  requests_data_.push(y);
}

void HighlightingToucher::PassiveMouseMove(int x, int y) {
  callback_requests_.push(MOUSE_MOVE);
  requests_data_.push(x);
  requests_data_.push(y);
}

bool HighlightingToucher::HasTouchPoint() {
  return !infinity_touch_;
}

void HighlightingToucher::GetTouchPoint(float* dst) {
  dst[0] = world_x_;
  dst[1] = world_y_;
  dst[2] = world_z_;
}

void HighlightingToucher::SolveTouchRequests() {
  while (!callback_requests_.empty()) {
    if (callback_requests_.front() == MOUSE_MOVE) {
      int x = requests_data_.front();
      requests_data_.pop();
      int y = requests_data_.front();
      requests_data_.pop();
      Toucher::ProcessTouch(x, y);
    }
    callback_requests_.pop();
  }
}
