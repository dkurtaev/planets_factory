#ifndef INCLUDE_CHANGE_COLOR_BUTTON_H_
#define INCLUDE_CHANGE_COLOR_BUTTON_H_

#include "include/button.h"
#include "include/palette_view.h"

class ChangeColorButton : public Button {
 public:
  ChangeColorButton();

  ~ChangeColorButton();

  virtual void MouseFunc(int button, int state, int x, int y);

  void GetSelectedColor(uint8_t* rgb) const;

 private:
  PaletteView* palette_view_;
};

#endif  // INCLUDE_CHANGE_COLOR_BUTTON_H_
