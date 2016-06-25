#include "include/value_palette_listener.h"

#include <iostream>

#include "include/palette_view.h"

ValuePaletteListener::ValuePaletteListener(PaletteView* palette_view)
  : palette_view_(palette_view) {}

void ValuePaletteListener::MouseMove(int x, int y) {
  palette_view_->SetValue(1.0 - static_cast<float>(y) / display_height_);
}

void ValuePaletteListener::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    palette_view_->SetValue(1.0 - static_cast<float>(y) / display_height_);
  }
}
