#ifndef INCLUDE_PALETTE_VIEW_H_
#define INCLUDE_PALETTE_VIEW_H_

#include "include/glview.h"

class PaletteView : public GLView {
 public:
  PaletteView();

  virtual void Display();
 
 private:
  static const int kViewHeight = 100;
  static const int kViewWidth = 200;
  static const int kColorCircleRadius = 50;

  void BuildColorCircle();

  void RGBtoHSV(float r, float g, float b, float* h, float* s, float* v);

  void HSVtoRGB(float h, float s, float v, float* r, float* g, float* b);

  int* points_array_;
  unsigned n_color_circle_points_;
  float* colors_array_;
};

#endif  // INCLUDE_PALETTE_VIEW_H_
