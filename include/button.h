#ifndef INCLUDE_BUTTON_H_
#define INCLUDE_BUTTON_H_

#include <string>
#include <iostream>

#include <GL/freeglut.h>

#include "include/glview_listener.h"

class Button : public GLViewListener {
 public:
  Button(std::string msg) {
    msg_ = msg;
  }

  virtual void MouseFunc(int button, int state, int x, int y) {
    if (!state) {
      std::cout << msg_ << std::endl;
    }
  }

  virtual void MouseMove(int x, int y) {
    glutSetCursor(GLUT_CURSOR_INFO);
  }

  virtual void PassiveMouseMove(int x, int y) {
    glutSetCursor(GLUT_CURSOR_INFO);
  }

  virtual void EntryFunc(int state) {
    if (state == GLUT_LEFT) {
      glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
    }
  }

 private:
  std::string msg_;
};

#endif  // INCLUDE_BUTTON_H_
