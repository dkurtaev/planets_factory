#include "include/hue_sat_palette_listener.h"

#include <iostream>

#include "include/palette_view.h"

HueSatPaletteListener::HueSatPaletteListener(PaletteView* palette_view)
  : palette_view_(palette_view) {}

void HueSatPaletteListener::MouseMove(int x, int y) {
  palette_view_->SetHueSaturation(
      static_cast<float>(x) / display_width_,
      1.0f - static_cast<float>(y) / display_height_);
}

void HueSatPaletteListener::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    palette_view_->SetHueSaturation(
        static_cast<float>(x) / display_width_,
        1.0f - static_cast<float>(y) / display_height_);
  }
}
