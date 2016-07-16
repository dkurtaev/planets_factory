// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef ENGINE_INCLUDE_BUTTON_H_
#define ENGINE_INCLUDE_BUTTON_H_

#include <stdint.h>

#include <string>

#include "include/glview_listener.h"

class Button : public GLViewListener {
 public:
  explicit Button(std::string text);

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
  uint8_t font_color_[3];
  uint8_t button_color_[3];
};

#endif  // ENGINE_INCLUDE_BUTTON_H_
