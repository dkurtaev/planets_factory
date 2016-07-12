// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/switcher.h"

#include <stdlib.h>
#include <string.h>

#include <string>

static const float kPressedButtonColor[3] = { 0.95f, 0.72f, 0.09f };

Switcher::Switcher(const std::string& text, bool* flag)
    : Button(text), flag_(flag) {
  memcpy(default_button_color_, button_color_, sizeof(float) * 3);
  SetFlag(flag_);
}

void Switcher::SetFlag(bool* flag) {
  if (flag != 0) {
    flag_ = flag;
    if (*flag_) {
      memcpy(button_color_, kPressedButtonColor, sizeof(float) * 3);      
    } 
  }
}

void Switcher::MouseFunc(int button, int state, int x, int y) {
  if (!state && flag_ != 0) {
    if (*flag_) {
      *flag_ = false;
      memcpy(button_color_, default_button_color_, sizeof(float) * 3);
    } else {
      *flag_ = true;
      memcpy(button_color_, kPressedButtonColor, sizeof(float) * 3);
    }
  }
}
