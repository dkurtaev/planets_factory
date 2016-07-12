// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_SWITCHER_H_
#define INCLUDE_SWITCHER_H_

#include <string>

#include "include/button.h"

// Button for switching custom boolean flag.
class Switcher : public Button {
 public:
  Switcher(const std::string& text, bool* flag = 0);

  void SetFlag(bool* flag);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  bool* flag_;
  float default_button_color_[3];
};

#endif  // INCLUDE_SWITCHER_H_
