// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/toucher.h"

#include <GL/freeglut.h>

Toucher::Toucher(Backtrace* backtrace)
  : left_button_pressed_(false), backtrace_(backtrace) {}

void Toucher::MouseFunc(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    left_button_pressed_ = !static_cast<bool>(state);
    if (left_button_pressed_) {
      InitAction();
      ProcessTouch(x, y);
    } else {
      FlushAction(backtrace_);
    }
  }
}

void Toucher::MouseMove(int x, int y) {
  if (left_button_pressed_) {
    ProcessTouch(x, y);
  }
}

void Toucher::DoEvents() {
  if (left_button_pressed_) {
    ProcessTouch(world_x_, world_y_, world_z_);
  }
}

void Toucher::ProcessTouch(int x, int y) {
  int view[4];
  double model[16];
  double proj[16];
  float z;
  glGetIntegerv(GL_VIEWPORT, view);
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);

  y = view[3] - y;
  glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  // If not infinity.
  if (z != 1.0f) {
    gluUnProject(x, y, z, model, proj, view, &world_x_, &world_y_, &world_z_);
    ProcessTouch(world_x_, world_y_, world_z_);
  }
}
