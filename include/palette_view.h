#ifndef INCLUDE_PALETTE_VIEW_H_
#define INCLUDE_PALETTE_VIEW_H_

#include <stdint.h>

#include "include/glview.h"
#include "include/layout.h"

class PaletteView : public GLView {
 public:
  PaletteView();

  virtual void Display();
 
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
  static const float kHSPaletteTop = 0.9f;
  static const float kHSPaletteLeft = 0.1f;
  static const float kHSPaletteRight = 0.7f;
  static const float kHSPaletteBottom = 0.1f;
  static const float kVPaletteTop = 0.9f;
  static const float kVPaletteLeft = 0.8f;
  static const float kVPaletteRight = 0.9f;
  static const float kVPaletteBottom = 0.1f;

  // h, s, v in [0, 1]
  // r, g, b in [0, 255]
  void HSVtoRGB(float h, float s, float v, uint8_t* colors_array_offset_);

  Layout layout_;
  int* points_array_;
  unsigned n_color_points_;
  uint8_t* colors_array_;
  uint8_t selected_solor_[3];
};

#endif  // INCLUDE_PALETTE_VIEW_H_
