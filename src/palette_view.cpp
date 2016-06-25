#include "include/palette_view.h"

#include <math.h>
#include <algorithm>

#include <GL/freeglut.h>

PaletteView::PaletteView()
  : GLView(kViewWidth, kViewHeight, "Select color") {
  const int hs_palette_top = kHSPaletteTop * kViewHeight;
  const int hs_palette_left = kHSPaletteLeft * kViewWidth;
  const int hs_palette_right = kHSPaletteRight * kViewWidth;
  const int hs_palette_bottom = kHSPaletteBottom * kViewHeight;
  const int hs_palette_width = (hs_palette_right - hs_palette_left + 1);
  const int hs_palette_height = (hs_palette_top - hs_palette_bottom + 1);

  n_color_points_ = hs_palette_width * hs_palette_height;
  points_array_ = new int[n_color_points_ * 2];
  colors_array_ = new uint8_t[n_color_points_ * 3];

  int* points_array_offset_ = points_array_;
  uint8_t* colors_array_offset_ = colors_array_;
  for (int y = hs_palette_bottom; y <= hs_palette_top; ++y) {
    for (int x = hs_palette_left; x <= hs_palette_right; ++x) {
      points_array_offset_[0] = x;
      points_array_offset_[1] = y;
      HSVtoRGB(static_cast<float>(x - hs_palette_left) / hs_palette_width,
               static_cast<float>(y - hs_palette_bottom) / hs_palette_height,
               1.0f, colors_array_offset_);
      points_array_offset_ += 2;
      colors_array_offset_ += 3;
    }
  }
}

void PaletteView::Display() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, kViewWidth, 0, kViewHeight, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors_array_);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_INT, 0, points_array_);
  glDrawArrays(GL_POINTS, 0, n_color_points_);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}

void PaletteView::HSVtoRGB(float h, float s, float v,
                           uint8_t* colors_array_offset_) {
  v *= 100.0f;
  float v_min = (1.0f - s) * v;
  float a = (static_cast<int>(h * 360.0f) % 60) * (v - v_min) / 60.0f;
  float v_inc = v_min + a;
  float v_dec = v - a;
  float rs[] = {v, v_dec, v_min, v_min, v_inc, v};
  float gs[] = {v_inc, v, v, v_dec, v_min, v_min};
  float bs[] = {v_min, v_min, v_inc, v, v, v_dec};
  int idx = static_cast<int>(h * 6) % 6;
  colors_array_offset_[0] = std::max(0.0f, std::min(rs[idx] * 2.55f, 255.0f));
  colors_array_offset_[1] = std::max(0.0f, std::min(gs[idx] * 2.55f, 255.0f));
  colors_array_offset_[2] = std::max(0.0f, std::min(bs[idx] * 2.55f, 255.0f));
}
