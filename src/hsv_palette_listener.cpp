#include "include/hsv_palette_listener.h"

#include "include/palette_view.h"

HSVPaletteListener::HSVPaletteListener(Target target, PaletteView* palette_view)
  : target_(target), palette_view_(palette_view) {}

void HSVPaletteListener::MouseMove(int x, int y) {
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

void HSVPaletteListener::MouseFunc(int button, int state, int x, int y) {
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
