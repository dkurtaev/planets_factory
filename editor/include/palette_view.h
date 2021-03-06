// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_PALETTE_VIEW_H_
#define EDITOR_INCLUDE_PALETTE_VIEW_H_

#include <stdint.h>

#include "include/glview.h"
#include "include/layout.h"

class PaletteListener;
class PaletteView : public GLView {
 public:
  explicit PaletteView(const PaletteView* palette_view = 0);

  ~PaletteView();

  virtual void Display();

  void SetHueSaturation(float hue, float saturation);

  // Value in [0, 1].
  void SetValue(float value);

  void GetSelectedColor(uint8_t* rgb) const;

 private:
  static const int kViewHeight = 300;
  static const int kViewWidth = 500;
  // Layout parameters.
  //  0.1      0.7
  // +------------------+
  // | +--------+ +---+ | 0.9
  // | |   HS   | | V | |
  // | +--------+ +---+ | 0.1
  // +------------------+
  //             0.8 0.9
  static const float kHSPaletteTop = kViewHeight * 0.9f;
  static const float kHSPaletteLeft = kViewWidth * 0.1f;
  static const float kHSPaletteRight = kViewWidth * 0.7f;
  static const float kHSPaletteBottom = kViewHeight * 0.1f;
  static const float kVPaletteTop = kViewHeight * 0.9f;
  static const float kVPaletteLeft = kViewWidth * 0.8f;
  static const float kVPaletteRight = kViewWidth * 0.9f;
  static const float kVPaletteBottom = kViewHeight * 0.1f;
  // Color selection markers.
  // Hue-Saturation plane, black circle.
  static const unsigned kHSPaletteMarkerRadius = 8;
  // Value bar marker, white triangle.
  // ....| ,
  // ....|/|
  // ....|\|
  // ....| '
  static const unsigned kVPaletteMarkerWidth = 8;

  // h, s, v in [0, 1]
  // r, g, b in [0, 255]
  void HSVtoRGB(float h, float s, float v, uint8_t* colors_array_offset_) const;

  void UpdateVPaletteColors();

  Layout layout_;
  PaletteListener* v_palette_listener_;
  PaletteListener* hs_palette_listener_;

  unsigned hs_palette_texture_id_;
  uint8_t* hs_palette_colors_;

  unsigned v_palette_texture_id_;
  uint8_t* v_palette_colors_;

  float selected_hsv_[3];
};

class PaletteListener : public GLViewListener {
 public:
  enum Target { HUESAT, VALUE };

  PaletteListener(Target target, PaletteView* palette_view);

  virtual void MouseMove(int x, int y);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  Target target_;
  PaletteView* palette_view_;
};

#endif  // EDITOR_INCLUDE_PALETTE_VIEW_H_
