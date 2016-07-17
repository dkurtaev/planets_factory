// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/switcher.h"

#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

#include <glog/logging.h>

static const uint8_t kPressedButtonColor[3] = { 242, 183, 23 };

Switcher::Switcher(const std::string& text, bool* flag)
    : Button(text), flag_(flag), radio_group_(0) {
  memcpy(default_button_color_, button_color_, sizeof(uint8_t) * 3);
  SetFlag(flag_);
}

void Switcher::SetFlag(bool* flag) {
  flag_ = flag;
  if (flag_ != 0 && *flag_) {
    memcpy(button_color_, kPressedButtonColor, sizeof(uint8_t) * 3);
  } else {
    memcpy(button_color_, default_button_color_, sizeof(uint8_t) * 3);
  }
}

void Switcher::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    Switch();
  }
}

void Switcher::AddToRadioGroup(std::vector<Switcher*>* group) {
  radio_group_ = group;
  group->push_back(this);
  if (flag_ != 0 && *flag_) {
    *flag_ = false;
    memcpy(button_color_, default_button_color_, sizeof(uint8_t) * 3);
  }
}

void Switcher::Switch() {
  if (flag_ == 0) return;
  if (*flag_) {
    *flag_ = false;
    memcpy(button_color_, default_button_color_, sizeof(uint8_t) * 3);
  } else {
    *flag_ = true;
    memcpy(button_color_, kPressedButtonColor, sizeof(uint8_t) * 3);

    if (radio_group_ != 0) {
      const unsigned n_radio_buttons = radio_group_->size();
      Switcher* switcher;
      for (unsigned i = 0; i < n_radio_buttons; ++i) {
        switcher = radio_group_->operator[](i);
        if (switcher != this && *switcher->flag_ != 0 && *switcher->flag_) {
          *switcher->flag_ = false;
          memcpy(switcher->button_color_, switcher->default_button_color_,
                 sizeof(uint8_t) * 3);
        }
      }
    }
  }
}
