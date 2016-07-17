// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_LOAD_BUTTON_H_
#define EDITOR_INCLUDE_LOAD_BUTTON_H_

#include "include/button.h"
#include "include/icosphere.h"
#include "include/console_view.h"

class LoadButton : public Button {
 public:
  LoadButton(Icosphere* icosphere, ConsoleView* console_view);

  virtual void MouseFunc(int button, int state, int x, int y);

  virtual void DoEvents();

 private:
  Icosphere* icosphere_;
  ConsoleView* console_view_;
};

#endif  // EDITOR_INCLUDE_LOAD_BUTTON_H_
