// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef ENGINE_INCLUDE_GLVIEW_LISTENER_H_
#define ENGINE_INCLUDE_GLVIEW_LISTENER_H_

#ifndef GLUT_WHEEL_UP
  #define GLUT_WHEEL_UP 3
#endif

#ifndef GLUT_WHEEL_DOWN
  #define GLUT_WHEEL_DOWN 4
#endif

#include <stdint.h>

class GLViewListener {
 public:
  GLViewListener() { is_enabled_ = true; }

  virtual void MouseFunc(int button, int state, int x, int y) {}

  virtual void MouseMove(int x, int y) {}

  virtual void PassiveMouseMove(int x, int y) {}

  virtual void SpecialKeyPressed(int key, int x, int y) {}

  virtual void SpecialKeyReleased(int key, int x, int y) {}

  virtual void KeyPressed(uint8_t key, int x, int y) {}

  virtual void DoEvents() {}

  virtual void EntryFunc(int state) {}

  virtual void Reshape(int display_width, int display_height) {
    display_width_ = display_width;
    display_height_ = display_height;
  }

  void Enable() { is_enabled_ = true; }

  void Disable() { is_enabled_ = false; }

  virtual bool IsEnabled() { return is_enabled_; }

 protected:
  float display_width_;
  float display_height_;
  bool is_enabled_;
};

#endif  // ENGINE_INCLUDE_GLVIEW_LISTENER_H_
