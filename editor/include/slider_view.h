// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_SLIDER_VIEW_H_
#define INCLUDE_SLIDER_VIEW_H_

#include <stdint.h>

#include "include/glview.h"
#include "include/slider_listener.h"

class SliderView : public GLView {
 public:
  explicit SliderView(const SliderView* slider_view = 0);

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
  SliderListener listener_;
};

#endif  // INCLUDE_SLIDER_VIEW_H_
