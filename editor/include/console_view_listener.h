// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_
#define EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_

#include <stdint.h>

#include <string>

#include "include/glview_listener.h"
#include "include/console_view.h"

class ConsoleViewListener : public GLViewListener {
 public:
  explicit ConsoleViewListener(ConsoleView* console_view);

  virtual void KeyPressed(uint8_t key, int x, int y);

 private:
  ConsoleView* console_view_;
};

#endif  // EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_
