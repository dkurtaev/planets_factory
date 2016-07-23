// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_LOAD_BUTTON_H_
#define EDITOR_INCLUDE_LOAD_BUTTON_H_

#include "include/button.h"
#include "include/icosphere.h"
#include "include/backtrace.h"

class LoadButton : public Button {
 public:
  // Clearing backtrace when loading new model.
  LoadButton(Icosphere* icosphere, Backtrace* backtrace);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  Icosphere* icosphere_;
  Backtrace* backtrace_;
};

#endif  // EDITOR_INCLUDE_LOAD_BUTTON_H_
