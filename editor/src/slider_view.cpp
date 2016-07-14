// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/slider_view.h"

#include <GL/freeglut.h>

const uint8_t SliderView::kMarkerColor[] = {0, 204, 0};
const uint8_t SliderView::kPowerConeColor[] = {204, 204, 0};

SliderView::SliderView(const SliderView* slider_view)
  : GLView(kViewWidth, kViewHeight), listener_(this) {
  AddListener(&listener_);
  power_ = (slider_view ? slider_view->power_ : 0.5f);
}

void SliderView::Display() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, kViewWidth, -kViewHeight / 2, kViewHeight / 2, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glLoadIdentity();

    unsigned marker_x = power_ * kViewWidth;
    glBegin(GL_TRIANGLES);
      // Power cone.
      glColor3ubv(kPowerConeColor);
      glVertex2i(0, 0);
      glVertex2i(marker_x, -0.5f * power_ * kViewHeight);
      glVertex2i(marker_x, 0.5f * power_ * kViewHeight);
      // Marker.
      glColor3ubv(kMarkerColor);
      glVertex2i(marker_x, 0);
      glVertex2i(marker_x - kMarkerWidth, -kMarkerWidth);
      glVertex2i(marker_x + kMarkerWidth, -kMarkerWidth);
    glEnd();
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}

float SliderView::GetPower() const {
  return power_;
}

void SliderView::SetPower(float power) {
  power_ = power;
}
