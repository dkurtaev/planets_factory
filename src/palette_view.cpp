#include "include/palette_view.h"

#include <math.h>
#include <iostream>

#include <GL/freeglut.h>

PaletteView::PaletteView()
  : GLView(kViewWidth, kViewHeight, "Select color") {
  BuildColorCircle();
}


void PaletteView::Display() {
  static const float kColorCircleRadius = 30;

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-kViewWidth / 2, kViewWidth / 2, -kViewHeight / 2, kViewHeight / 2,
          0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glBegin(GL_POINTS);
  float r, g, b;
  for (unsigned i = 0; i < n_color_circle_points_; ++i) {
    glColor3fv(colors_array_ + i * 3);
    glVertex2iv(points_array_ + i * 2);
  }
  glEnd();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}

void PaletteView::RGBtoHSV(float r, float g, float b, float* h, float* s,
                           float* v) {
  static const float kRatio = 60.0f / 360.0f;
  float max_rgb = std::max(std::max(r, g), b);
  float min_rgb = std::min(std::min(r, g), b);
  *h = 0;
  if (max_rgb != min_rgb) {
    if (max_rgb == r) {
      *h = kRatio * (g - b) / (max_rgb - min_rgb) + (g >= b ? 0 : 1);
    } else if (max_rgb == g) {
      *h = kRatio * ((b - r) / (max_rgb - min_rgb) + 2);
    } else {
      *h = kRatio * ((r - g) / (max_rgb - min_rgb) + 4);
    }
  }
  *s = (max_rgb != 0 ? 1 - min_rgb / max_rgb : 0);
  *v = max_rgb;
}

void PaletteView::HSVtoRGB(float h, float s, float v, float* r, float* g,
                           float* b) {
  v *= 100.0f;
  int idx = static_cast<int>(h * 6) % 6;
  float v_min = (1.0f - s) * v;
  float a = (static_cast<int>(h * 360.0f) % 60) * (v - v_min) / 60.0f;
  float v_inc = v_min + a;
  float v_dec = v - a;
  float rs[] = {v, v_dec, v_min, v_min, v_inc, v};
  float gs[] = {v_inc, v, v, v_dec, v_min, v_min};
  float bs[] = {v_min, v_min, v_inc, v, v, v_dec};
  *r = rs[idx] / 255.0f;
  *g = gs[idx] / 255.0f;
  *b = bs[idx] / 255.0f;
}

void PaletteView::BuildColorCircle() {
  int x = kColorCircleRadius;
  int y = 0;
  int* right_xs = new int[kColorCircleRadius + 1];
  right_xs[0] = x;
  n_color_circle_points_ = 4 * kColorCircleRadius + 1;
  do {
    int term = kColorCircleRadius * kColorCircleRadius - y * y - x * x;
    int diff_up = abs(term - 2 * y - 1);
    int diff_left = abs(term + 2 * x - 1);
    int diff_up_left = abs(term + 2 * x - 2 * y - 2);
    if (diff_left <= diff_up) {
      --x;
      if (diff_up_left <= diff_left) {
        right_xs[++y] = x;
        n_color_circle_points_ += 4 * x;
      }
    } else {
      if (diff_up_left <= diff_up) {
        --x;
      }
      right_xs[++y] = x;
      n_color_circle_points_ += 4 * x;
    }
  } while (x != 0);

  points_array_ = new int[2 * n_color_circle_points_];
  points_array_[0] = 0;
  points_array_[1] = 0;
  unsigned idx = 2;
  for (int x = 1; x <= right_xs[0]; ++x) {
    points_array_[idx++] = x;
    points_array_[idx++] = 0;
    points_array_[idx++] = -x;
    points_array_[idx++] = 0;    

    points_array_[idx++] = 0;
    points_array_[idx++] = x;    
    points_array_[idx++] = 0;
    points_array_[idx++] = -x;
  }

  for (int y = 1; y <= kColorCircleRadius; ++y) {
    for (int x = 1; x <= right_xs[y]; ++x) {
      points_array_[idx++] = x;
      points_array_[idx++] = y;
      points_array_[idx++] = -x;
      points_array_[idx++] = y;
      points_array_[idx++] = x;
      points_array_[idx++] = -y;
      points_array_[idx++] = -x;
      points_array_[idx++] = -y;
    }
  }
  delete[] right_xs;

  if (idx != 2 * n_color_circle_points_) {
    std::cout << "Mistake in PaletteView::BuildColorCircle()" << std::endl;
  }

  colors_array_ = new float[n_color_circle_points_ * 3];
  for (unsigned i = 0; i < n_color_circle_points_; ++i) {
    float x = points_array_[i * 2];
    float y = points_array_[i * 2 + 1];
    float h = (atan2(y, x) + M_PI) * 0.5 / M_PI;
    float s = sqrt(x * x + y * y);
    HSVtoRGB(h, s, 1.0f, colors_array_ + i * 3, colors_array_ + i * 3 + 1,
             colors_array_ + i * 3 + 2);
  }
}
 