// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_SAVE_BUTTON_H_
#define EDITOR_INCLUDE_SAVE_BUTTON_H_

#include "include/button.h"
#include "include/icosphere.h"
#include "include/console_view_listener.h"

class SaveButton : public Button {
 public:
  explicit SaveButton(const Icosphere* icosphere,
                      ConsoleViewListener* console_view_listener);

  virtual void MouseFunc(int button, int state, int x, int y);

  virtual void DoEvents();

 private:
  const Icosphere* icosphere_;
  ConsoleViewListener* console_view_listener_;
  // If true, check console listener for new command (saving path expected).
  bool waiting_path_;
};

#endif  // EDITOR_INCLUDE_SAVE_BUTTON_H_
