// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/palette_view.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

#include <algorithm>

#include <GL/freeglut.h>

PaletteView::PaletteView(const PaletteView* palette_view)
  : GLView(kViewWidth, kViewHeight, "Select color") {
  const int hs_palette_width = (kHSPaletteRight - kHSPaletteLeft + 1);
  const int hs_palette_height = (kHSPaletteTop - kHSPaletteBottom + 1);
  const int v_palette_width = (kVPaletteRight - kVPaletteLeft + 1);
  const int v_palette_height = (kVPaletteTop - kVPaletteBottom + 1);

  v_palette_listener_ = new PaletteListener(PaletteListener::VALUE, this);
  hs_palette_listener_ = new PaletteListener(PaletteListener::HUESAT, this);

  hs_palette_colors_ = new uint8_t[hs_palette_width * hs_palette_height * 3];
  v_palette_colors_ = new uint8_t[v_palette_width * v_palette_height * 3];

  glGenTextures(1, &hs_palette_texture_id_);
  glBindTexture(GL_TEXTURE_2D, hs_palette_texture_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenTextures(1, &v_palette_texture_id_);
  glBindTexture(GL_TEXTURE_2D, v_palette_texture_id_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (palette_view) {
    memcpy(hs_palette_colors_, palette_view->hs_palette_colors_,
           sizeof(uint8_t) * hs_palette_width * hs_palette_height * 3);
    memcpy(v_palette_colors_, palette_view->v_palette_colors_,
           sizeof(uint8_t) * v_palette_width * v_palette_height * 3);
    memcpy(selected_hsv_, palette_view->selected_hsv_, sizeof(float) * 3);
  } else {
    uint8_t* colors_array_offset_ = hs_palette_colors_;
    for (int y = kHSPaletteBottom; y <= kHSPaletteTop; ++y) {
      for (int x = kHSPaletteLeft; x <= kHSPaletteRight; ++x) {
        HSVtoRGB(static_cast<float>(x - kHSPaletteLeft) / hs_palette_width,
                 static_cast<float>(y - kHSPaletteBottom) / hs_palette_height,
                 1.0f, colors_array_offset_);
        colors_array_offset_ += 3;
      }
    }
    selected_hsv_[0] = 0.5f;
    selected_hsv_[1] = 0.5f;
    selected_hsv_[2] = 0.5f;
    UpdateVPaletteColors();
  }

  glBindTexture(GL_TEXTURE_2D, hs_palette_texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hs_palette_width, hs_palette_height,
               0, GL_RGB, GL_UNSIGNED_BYTE, hs_palette_colors_);

  glBindTexture(GL_TEXTURE_2D, v_palette_texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, v_palette_width, v_palette_height,
               0, GL_RGB, GL_UNSIGNED_BYTE, v_palette_colors_);

  Roi v_palette_roi(static_cast<float>(kVPaletteLeft) / kViewWidth,
                    static_cast<float>(kVPaletteRight) / kViewWidth,
                    static_cast<float>(kVPaletteBottom) / kViewHeight,
                    static_cast<float>(kVPaletteTop) / kViewHeight);
  layout_.AddListener(v_palette_listener_, v_palette_roi);

  Roi hs_palette_roi(static_cast<float>(kHSPaletteLeft) / kViewWidth,
                     static_cast<float>(kHSPaletteRight) / kViewWidth,
                     static_cast<float>(kHSPaletteBottom) / kViewHeight,
                     static_cast<float>(kHSPaletteTop) / kViewHeight);
  layout_.AddListener(hs_palette_listener_, hs_palette_roi);
  AddListener(&layout_);
}

PaletteView::~PaletteView() {
  delete[] hs_palette_colors_;
  delete[] v_palette_colors_;
  delete v_palette_listener_;
  delete hs_palette_listener_;
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

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    glBindTexture(GL_TEXTURE_2D, hs_palette_texture_id_);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 1); glVertex2i(kHSPaletteLeft, kHSPaletteTop);
      glTexCoord2f(0, 0); glVertex2i(kHSPaletteLeft, kHSPaletteBottom);
      glTexCoord2f(1, 0); glVertex2i(kHSPaletteRight, kHSPaletteBottom);
      glTexCoord2f(1, 1); glVertex2i(kHSPaletteRight, kHSPaletteTop);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, v_palette_texture_id_);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 1); glVertex2i(kVPaletteLeft, kVPaletteTop);
      glTexCoord2f(0, 0); glVertex2i(kVPaletteLeft, kVPaletteBottom);
      glTexCoord2f(1, 0); glVertex2i(kVPaletteRight, kVPaletteBottom);
      glTexCoord2f(1, 1); glVertex2i(kVPaletteRight, kVPaletteTop);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    const int hs_palette_width = (kHSPaletteRight - kHSPaletteLeft + 1);
    const int hs_palette_height = (kHSPaletteTop - kHSPaletteBottom + 1);
    glPointSize(kHSPaletteMarkerRadius);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
      glVertex2i(kHSPaletteLeft + selected_hsv_[0] * hs_palette_width,
                 kHSPaletteBottom + selected_hsv_[1] * hs_palette_height);
    glEnd();

    const int v_palette_height = (kVPaletteTop - kVPaletteBottom + 1);
    const int tangency_point_y = kVPaletteBottom + selected_hsv_[2] *
                                 v_palette_height;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
      glVertex2i(kVPaletteRight, tangency_point_y);
      glVertex2i(kVPaletteRight + kVPaletteMarkerWidth,
                 tangency_point_y - kVPaletteMarkerWidth);
      glVertex2i(kVPaletteRight + kVPaletteMarkerWidth,
                 tangency_point_y + kVPaletteMarkerWidth);
    glEnd();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}

void PaletteView::HSVtoRGB(float h, float s, float v,
                           uint8_t* colors_array_offset_) const {
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
  uint8_t top_rgb[3];
  HSVtoRGB(selected_hsv_[0], selected_hsv_[1], 1.0f, top_rgb);

  const int v_palette_width = (kVPaletteRight - kVPaletteLeft + 1);
  const int v_palette_height = (kVPaletteTop - kVPaletteBottom + 1);

  uint8_t* colors_array_offset_ = v_palette_colors_ - 3;
  for (int y = kVPaletteBottom; y <= kVPaletteTop; ++y) {
    float ratio = static_cast<float>(y - kVPaletteBottom) / v_palette_height;
    uint8_t rgb[] = {
      top_rgb[0] * ratio,
      top_rgb[1] * ratio,
      top_rgb[2] * ratio
    };
    for (int x = kVPaletteLeft; x <= kVPaletteRight; ++x) {
      memcpy(colors_array_offset_ += 3, rgb, sizeof(uint8_t) * 3);
    }
  }
  glBindTexture(GL_TEXTURE_2D, v_palette_texture_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, v_palette_width, v_palette_height,
               0, GL_RGB, GL_UNSIGNED_BYTE, v_palette_colors_);
}

void PaletteView::SetValue(float value) {
  selected_hsv_[2] = std::max(0.0f, std::min(value, 1.0f));
}

void PaletteView::SetHueSaturation(float hue, float saturation) {
  selected_hsv_[0] = std::max(0.0f, std::min(hue, 1.0f));
  selected_hsv_[1] = std::max(0.0f, std::min(saturation, 1.0f));
  UpdateVPaletteColors();
}

void PaletteView::GetSelectedColor(uint8_t* rgb) const {
  HSVtoRGB(selected_hsv_[0], selected_hsv_[1], selected_hsv_[2], rgb);
}

PaletteListener::PaletteListener(Target target, PaletteView* palette_view)
  : target_(target), palette_view_(palette_view) {}

void PaletteListener::MouseMove(int x, int y) {
  switch (target_) {
    case HUESAT:
      palette_view_->SetHueSaturation(
          static_cast<float>(x) / display_width_,
          1.0f - static_cast<float>(y) / display_height_);
      break;
    case VALUE:
      palette_view_->SetValue(1.0 - static_cast<float>(y) / display_height_);
      break;
    default: break;
  }
}

void PaletteListener::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    switch (target_) {
      case HUESAT:
        palette_view_->SetHueSaturation(
            static_cast<float>(x) / display_width_,
            1.0f - static_cast<float>(y) / display_height_);
        break;
      case VALUE:
        palette_view_->SetValue(1.0 - static_cast<float>(y) / display_height_);
        break;
      default: break;
    }
  }
}
