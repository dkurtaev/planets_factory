// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/planet_view.h"

#include <string>

#include <GL/freeglut.h>
#include <glog/logging.h>

#include "include/shaders_factory.h"

PlanetView::PlanetView(const Icosphere* icosphere, SphericalCS* camera_cs,
                       const cv::Mat* texture, bool* draw_grid)
  : GLView(500, 500, "Planets factory"), icosphere_(icosphere),
    camera_(camera_cs), texture_(texture), draw_grid_(draw_grid) {
  InitGL();
  planet_shader_program_ = ShadersFactory::GetProgramFromFile(
                               "../res/shaders/test_shader.vertex",
                               "../res/shaders/test_shader.fragment");
  grid_shader_program_ = ShadersFactory::GetProgramFromFile(
                             "../res/shaders/icogrid_shader.vertex",
                             "../res/shaders/icogrid_shader.fragment");
  SetTexture();
}

void PlanetView::Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera_.Setup(display_width_, display_height_);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glUseProgram(planet_shader_program_);
  //
  float modelview_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
  unsigned loc = glGetUniformLocation(planet_shader_program_,
                                      "u_modelview_matrix");
  glUniformMatrix4fv(loc, 1, false, modelview_matrix);

  float projection_matrix[16];
  glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);
  loc = glGetUniformLocation(planet_shader_program_, "u_projection_matrix");
  glUniformMatrix4fv(loc, 1, false, projection_matrix);

  loc = glGetUniformLocation(planet_shader_program_, "u_light_vector");
  glUniform3f(loc, -100, -100, -100);

  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_->cols, texture_->rows,
               0, GL_RGB, GL_UNSIGNED_BYTE, texture_->data);
  glUniform1i(3, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  //
  icosphere_->Draw();

  if (*draw_grid_) {
    glUseProgram(grid_shader_program_);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
    loc = glGetUniformLocation(grid_shader_program_, "u_modelview_matrix");
    glUniformMatrix4fv(loc, 1, false, modelview_matrix);

    glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);
    loc = glGetUniformLocation(grid_shader_program_, "u_projection_matrix");
    glUniformMatrix4fv(loc, 1, false, projection_matrix);

    icosphere_->DrawGrid();
  }

  glUseProgram(0);  // Disable shader program.

  glBegin(GL_LINES);
  for (int i = 0; i < 3; ++i) {
    glColor3f(i == 0, i == 1, i == 2);
    glVertex3f(0, 0, 0);
    glVertex3f(i == 0 ? 100 : 0, i == 1 ? 100 : 0, i == 2 ? 100 : 0);
  }
  glEnd();

  glutSwapBuffers();
}

void PlanetView::InitGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  glEnable(GL_LIGHT0);
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1 };
  GLfloat position[] = { 100, 100, 100, 1 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void PlanetView::SetTexture() {
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
