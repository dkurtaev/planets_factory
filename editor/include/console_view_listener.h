// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_
#define EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_

#include <stdint.h>

#include "include/glview_listener.h"
#include "include/console_view.h"

class ConsoleViewListener : public GLViewListener {
 public:
  explicit ConsoleViewListener(ConsoleView* console_view);

  virtual void KeyPressed(uint8_t key, int x, int y);

  // Returns last command and clear command_ member (command is processed).
  std::string ProcessCommand();

 private:
  ConsoleView* console_view_;
  std::string command_;  // Text after pressing Enter key is a command.
};

#endif  // EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_
