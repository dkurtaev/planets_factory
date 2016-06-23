#ifndef INCLUDE_LISTENER_ENABLER_H_
#define INCLUDE_LISTENER_ENABLER_H_

#include <string>

#include "include/glview_listener.h"
#include "include/button.h"

class ListenerEnabler : public Button {
 public:
  ListenerEnabler(std::string text, GLViewListener* listener);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  GLViewListener* listener_;
  float default_button_color_[3];
};

#endif  // INCLUDE_LISTENER_ENABLER_H_
