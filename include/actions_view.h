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
  std::vector<std::string> actions_titles_;
};

#endif  // INCLUDE_ACTIONS_VIEW_H_
