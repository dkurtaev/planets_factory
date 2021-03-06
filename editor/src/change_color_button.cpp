// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/change_color_button.h"

ChangeColorButton::ChangeColorButton()
  : Button("Palette") {
  palette_view_ = new PaletteView();
  palette_view_->Hide();
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
  palette_view_->GetSelectedColor(rgb);
}
