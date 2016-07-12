// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_ACTIONS_VIEW_H_
#define INCLUDE_ACTIONS_VIEW_H_

#include <vector>

#include "include/glview.h"
#include "include/button.h"
#include "include/layout.h"

class ActionsView : public GLView {
 public:
  ActionsView(std::vector<Button*> buttons, GLView* parent = 0, int sub_x = 0,
              int sub_y = 0);

  virtual void Display();

 private:
  static const unsigned kInitButtonRoiWidth = 150;
  static const unsigned kInitButtonRoiHeight = 50;
  // Idents of drawing buttons from ROI borders.
  static const float kLeftIdent = 0.1f;
  static const float kRightIdent = 0.1f;
  static const float kTopIdent = 0.2f;
  static const float kBottomIdent = 0.2f;

  std::vector<Button*> buttons_;
  Layout layout_;
  std::vector<Roi> buttons_rois_;
};

#endif  // INCLUDE_ACTIONS_VIEW_H_
