#ifndef INCLUDE_BUTTON_H_
#define INCLUDE_BUTTON_H_

#include <string>
#include <iostream>

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

 private:
  std::string msg_;
};

#endif  // INCLUDE_BUTTON_H_
