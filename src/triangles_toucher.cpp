// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/triangles_toucher.h"

#include <math.h>

#include <utility>
#include <algorithm>
#include <vector>

#include <GL/freeglut.h>

TrianglesToucher::TrianglesToucher(std::vector<Triangle*>* triangles)
  : triangles_(triangles), left_button_pressed_(false) {}

void TrianglesToucher::MouseFunc(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    left_button_pressed_ = !static_cast<bool>(state);
    if (left_button_pressed_) {
      ProcessTouch(x, y);
    }
  }
}

void TrianglesToucher::MouseMove(int x, int y) {
  if (left_button_pressed_) {
    ProcessTouch(x, y);
  }
}

void TrianglesToucher::ProcessTouch(int x, int y) {
  int view[4];
  double model[16];
  double proj[16];
  float z;
  double world[3];
  glGetIntegerv(GL_VIEWPORT, view);
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);

  y = view[3] - y;
  glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  if (z == 1.0f) return;  // Infinity.

  gluUnProject(x, y, z, model, proj, view, world, world + 1, world + 2);

  // Find triangle.
  const unsigned n_tris = triangles_->size();
  Triangle* triangle;
  float bary_p1, bary_p2, bary_p3;
  for (unsigned i = 0; i < n_tris; ++i) {
    triangle = triangles_->operator[](i);
    if (triangle->IsIncludes(world[0], world[1], world[2],
                             &bary_p1, &bary_p2, &bary_p3)) {
      DoAction(triangle, bary_p1, bary_p2, bary_p3);
      break;
    }
  }
}
