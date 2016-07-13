// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/slider_listener.h"

#include "include/slider_view.h"

SliderListener::SliderListener(SliderView* slider_view)
  : slider_view_(slider_view) {}

void SliderListener::MouseMove(int x, int y) {
  float power = static_cast<float>(x) / display_width_;
  power = std::max(0.0f, std::min(power, 1.0f));
  slider_view_->SetPower(power);
}

void SliderListener::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    slider_view_->SetPower(static_cast<float>(x) / display_width_);
  }
}
