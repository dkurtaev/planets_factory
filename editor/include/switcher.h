// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_SWITCHER_H_
#define EDITOR_INCLUDE_SWITCHER_H_

#include <stdint.h>

#include <string>
#include <vector>

#include "include/button.h"

// Button for switching custom boolean flag.
class Switcher : public Button {
 public:
  explicit Switcher(const std::string& text, bool* flag = 0);

  void SetFlag(bool* flag);

  virtual void MouseFunc(int button, int state, int x, int y);

  void Switch();

  // If enabled current button disable others.
  void AddToRadioGroup(std::vector<Switcher*>* group);

 private:
  bool* flag_;
  uint8_t default_button_color_[3];
  std::vector<Switcher*>* radio_group_;
};

#endif  // EDITOR_INCLUDE_SWITCHER_H_
