// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_LOAD_BUTTON_H_
#define EDITOR_INCLUDE_LOAD_BUTTON_H_

#include "include/button.h"
#include "include/icosphere.h"

class LoadButton : public Button {
 public:
  explicit LoadButton(Icosphere* icosphere);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  Icosphere* icosphere_;
};

#endif  // EDITOR_INCLUDE_LOAD_BUTTON_H_
