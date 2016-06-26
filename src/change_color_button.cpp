#include "include/change_color_button.h"

#include <iostream>

ChangeColorButton::ChangeColorButton()
  : Button("Change color") {
  palette_view_ = new PaletteView();
}

ChangeColorButton::~ChangeColorButton() {
  delete palette_view_;
}

void ChangeColorButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    PaletteView* new_palette_view_ = new PaletteView(palette_view_);
    delete palette_view_;
    palette_view_ = new_palette_view_;
  }
}

void ChangeColorButton::GetSelectedColor(uint8_t* rgb) const {
  if (palette_view_) {
    palette_view_->GetSelectedColor(rgb);
  }
}
