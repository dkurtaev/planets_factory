// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_SLIDER_VIEW_H_
#define EDITOR_INCLUDE_SLIDER_VIEW_H_

#include <stdint.h>

#include "include/glview.h"
#include "include/glview_listener.h"

class SliderListener;
class SliderView : public GLView {
 public:
  explicit SliderView(const SliderView* slider_view = 0);

  ~SliderView();

  virtual void Display();

  float GetPower() const;

  void SetPower(float power);

 private:
  static const int kViewHeight = 50;
  static const int kViewWidth = 200;
  static const int kMarkerWidth = 10;
  static const uint8_t kMarkerColor[];
  static const uint8_t kPowerConeColor[];

  // Value in [0, 1].
  float power_;
  SliderListener* listener_;
};

class SliderListener : public GLViewListener {
 public:
  explicit SliderListener(SliderView* slider_view);

  virtual void MouseMove(int x, int y);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  SliderView* slider_view_;
};

#endif  // EDITOR_INCLUDE_SLIDER_VIEW_H_
