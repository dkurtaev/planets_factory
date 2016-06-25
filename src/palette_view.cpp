#include "include/palette_view.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

#include <algorithm>

#include <GL/freeglut.h>

PaletteView::PaletteView()
  : GLView(kViewWidth, kViewHeight, "Select color") {
  const int hs_palette_width = (kHSPaletteRight - kHSPaletteLeft + 1);
  const int hs_palette_height = (kHSPaletteTop - kHSPaletteBottom + 1);
  const int v_palette_width = (kVPaletteRight - kVPaletteLeft + 1);
  const int v_palette_height = (kVPaletteTop - kVPaletteBottom + 1);

  n_hs_palette_points_ = hs_palette_width * hs_palette_height;
  n_v_palette_points_ = v_palette_width * v_palette_height;
  hs_palette_points_ = new int[n_hs_palette_points_ * 2];
  hs_palette_colors_ = new uint8_t[n_hs_palette_points_ * 3];
  v_palette_points_ = new int[n_v_palette_points_ * 2];
  v_palette_colors_ = new uint8_t[n_v_palette_points_ * 3];

  int* points_array_offset_ = hs_palette_points_;
  uint8_t* colors_array_offset_ = hs_palette_colors_;
  for (int y = kHSPaletteBottom; y <= kHSPaletteTop; ++y) {
    for (int x = kHSPaletteLeft; x <= kHSPaletteRight; ++x) {
      points_array_offset_[0] = x;
      points_array_offset_[1] = y;
      HSVtoRGB(static_cast<float>(x - kHSPaletteLeft) / hs_palette_width,
               static_cast<float>(y - kHSPaletteBottom) / hs_palette_height,
               1.0f, colors_array_offset_);
      points_array_offset_ += 2;
      colors_array_offset_ += 3;
    }
  }

  unsigned idx = 0;
  for (int y = kVPaletteBottom; y <= kVPaletteTop; ++y) {
    for (int x = kVPaletteLeft; x <= kVPaletteRight; ++x) {
      v_palette_points_[idx++] = x;
      v_palette_points_[idx++] = y;
    }
  }

  selected_hsv_[0] = 0.5f;
  selected_hsv_[1] = 0.5f;
  selected_hsv_[2] = 0.5f;

  UpdateVPaletteColors();
}

PaletteView::~PaletteView() {
  delete[] hs_palette_points_;
  delete[] hs_palette_colors_;
  delete[] v_palette_points_;
  delete[] v_palette_colors_;
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
  glColorPointer(3, GL_UNSIGNED_BYTE, 0, hs_palette_colors_);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_INT, 0, hs_palette_points_);
  glDrawArrays(GL_POINTS, 0, n_hs_palette_points_);

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_UNSIGNED_BYTE, 0, v_palette_colors_);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_INT, 0, v_palette_points_);
  glDrawArrays(GL_POINTS, 0, n_v_palette_points_);

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

void PaletteView::UpdateVPaletteColors() {
  const int v_palette_height = (kVPaletteTop - kVPaletteBottom + 1);
  uint8_t rgb[3];
  uint8_t* colors_array_offset_ = v_palette_colors_;
  for (int y = kVPaletteBottom; y <= kVPaletteTop; ++y) {
    HSVtoRGB(selected_hsv_[0], selected_hsv_[1],
             static_cast<float>(y - kVPaletteBottom) / v_palette_height, rgb);
    for (int x = kVPaletteLeft; x <= kVPaletteRight; ++x) {
      memcpy(colors_array_offset_, rgb, sizeof(uint8_t) * 3);
      colors_array_offset_ += 3;
    }
  }
}
