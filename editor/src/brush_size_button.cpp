// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/brush_size_button.h"

#include <math.h>

#include <algorithm>

BrushSizeButton::BrushSizeButton()
  : Button("Brush size") {
  slider_view_ = new SliderView();
  slider_view_->Hide();
}

BrushSizeButton::~BrushSizeButton() {
  delete slider_view_;
}

void BrushSizeButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    SliderView* new_slider_view_ = new SliderView(slider_view_);
    delete slider_view_;
    slider_view_ = new_slider_view_;
  }
}

unsigned BrushSizeButton::GetBrushSize(unsigned max_brush_size) const {
  static unsigned kMinBrushSize = 1;
  float power = slider_view_->GetPower();
  unsigned brush_size = pow(power, 2) * max_brush_size;
  brush_size = std::max(kMinBrushSize, brush_size);
  return brush_size;
}
