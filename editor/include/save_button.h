// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_SAVE_BUTTON_H_
#define EDITOR_INCLUDE_SAVE_BUTTON_H_

#include <string>

#include "include/button.h"
#include "include/icosphere.h"

class SaveButton : public Button {
 public:
  explicit SaveButton(const Icosphere* icosphere);

  virtual void MouseFunc(int button, int state, int x, int y);

  void Save();

 private:
  const Icosphere* icosphere_;
  std::string last_path_;
};

#endif  // EDITOR_INCLUDE_SAVE_BUTTON_H_
