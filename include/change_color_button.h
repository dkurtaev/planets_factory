#ifndef INCLUDE_CHANGE_COLOR_BUTTON_H_
#define INCLUDE_CHANGE_COLOR_BUTTON_H_

#include "include/button.h"

class ChangeColorButton : public Button {
 public:
  ChangeColorButton();

  virtual void MouseFunc(int button, int state, int x, int y);
};

#endif  // INCLUDE_CHANGE_COLOR_BUTTON_H_
