#ifndef INCLUDE_ACTIONS_VIEW_H_
#define INCLUDE_ACTIONS_VIEW_H_

#include <vector>
#include <string>

#include "include/glview.h"

class ActionsView : public GLView {
 public:
  ActionsView(const std::vector<GLViewListener*>& actions,
              const std::vector<std::string>& actions_titles);

  virtual void Display();

 private:
  static const unsigned kInitButtonRoiWidth = 100;
  static const unsigned kInitButtonRoiHeight = 50;
  // Idents of drawing buttons from ROI borders.
  static const float kLeftIdent = 0.1f;
  static const float kRightIdent = 0.1f;
  static const float kTopIdent = 0.1f;
  static const float kBottomIdent = 0.1f;

  std::vector<std::string> actions_titles_;
};

#endif  // INCLUDE_ACTIONS_VIEW_H_
