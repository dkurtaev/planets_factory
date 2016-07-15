// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef ENGINE_INCLUDE_TOUCHER_H_
#define ENGINE_INCLUDE_TOUCHER_H_

#include "include/glview_listener.h"

class Toucher : public GLViewListener {
 public:
  Toucher();

  virtual void MouseFunc(int button, int state, int x, int y);

  virtual void MouseMove(int x, int y);

 protected:
  virtual void ProcessTouch(float x, float y, float z) = 0;

 private:
  void ProcessTouch(int x, int y);

  bool left_button_pressed_;
};

#endif  // ENGINE_INCLUDE_TOUCHER_H_
