#ifndef INCLUDE_LISTENER_ENABLER_H_
#define INCLUDE_LISTENER_ENABLER_H_

#include "include/glview_listener.h"
#include "include/button.h"

#include <iostream>

class ListenerEnabler : public Button {
 public:
  ListenerEnabler(std::string text, GLViewListener* listener)
    : Button(text), listener_(listener) {}
 
  virtual void MouseFunc(int button, int state, int x, int y) {
    if (!state) {
      if (listener_->IsEnabled()) {
        listener_->Disable();
        std::cout << text_ << " disabled" << std::endl;
      } else {
        listener_->Enable();
        std::cout << text_ << " enabled" << std::endl;
      }      
    }
  }

 private:
  GLViewListener* listener_;
};

#endif  // INCLUDE_LISTENER_ENABLER_H_
