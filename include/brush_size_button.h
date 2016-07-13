// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_BRUSH_SIZE_BUTTON_H_
#define INCLUDE_BRUSH_SIZE_BUTTON_H_

#include "include/button.h"
#include "include/slider_view.h"

class BrushSizeButton : public Button {
 public:
  BrushSizeButton();

  ~BrushSizeButton();

  virtual void MouseFunc(int button, int state, int x, int y);

  unsigned GetBrushSize(unsigned max_brush_size) const;

 private:
  SliderView* slider_view_;
};

#endif  // INCLUDE_BRUSH_SIZE_BUTTON_H_
