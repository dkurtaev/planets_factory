#ifndef INCLUDE_BUTTON_H_
#define INCLUDE_BUTTON_H_

#include <string>

#include <GL/freeglut.h>

#include "include/glview_listener.h"

class Button : public GLViewListener {
 public:
  Button(std::string text);

  virtual void MouseMove(int x, int y);

  virtual void PassiveMouseMove(int x, int y);

  virtual void EntryFunc(int state);

  // Expected coordinate system in pixels:
  //     |(0, 0)
  // ----+----> x
  //     |
  //     v  y
  void Display(int semiwidth, int semiheight);

 private:
  std::string text_;
};

#endif  // INCLUDE_BUTTON_H_
