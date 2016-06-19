#ifndef INCLUDE_BUTTON_H_
#define INCLUDE_BUTTON_H_

#include <string>

#include "include/glview_listener.h"

class Button : public GLViewListener {
 public:
  Button(std::string text);

  virtual void MouseMove(int x, int y);

  virtual void PassiveMouseMove(int x, int y);

  virtual void EntryFunc(int state);

  virtual void MouseFunc(int button, int state, int x, int y) = 0;

  // Expected coordinate system in pixels:
  //     |(0, 0)
  // ----+----> x
  //     |
  //     v  y
  void Display(int semiwidth, int semiheight);

 protected:
  std::string text_;
  float font_color_[3];
  float button_color_[3];
};

#endif  // INCLUDE_BUTTON_H_
