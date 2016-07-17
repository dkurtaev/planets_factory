// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_SAVE_BUTTON_H_
#define EDITOR_INCLUDE_SAVE_BUTTON_H_

#include "include/button.h"
#include "include/icosphere.h"
#include "include/console_view.h"

class SaveButton : public Button {
 public:
  SaveButton(const Icosphere* icosphere, ConsoleView* console_view);

  // Tip console command or save by last path. New command overwrites last path.
  virtual void MouseFunc(int button, int state, int x, int y);

  // Parse console for "save" command.
  virtual void DoEvents();

  void Save();

 private:
  const Icosphere* icosphere_;
  ConsoleView* console_view_;
  std::string last_path_;
};

#endif  // EDITOR_INCLUDE_SAVE_BUTTON_H_
