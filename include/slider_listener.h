// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_SLIDER_LISTENER_H_
#define INCLUDE_SLIDER_LISTENER_H_

#include "include/glview_listener.h"

class SliderView;
class SliderListener : public GLViewListener {
 public:
  SliderListener(SliderView* slider_view);

  virtual void MouseMove(int x, int y);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  SliderView* slider_view_;
};

#endif  // INCLUDE_SLIDER_LISTENER_H_
