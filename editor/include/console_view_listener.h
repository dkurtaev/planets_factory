// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_
#define EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_

#include <stdint.h>

#include <string>

#include "include/glview_listener.h"
#include "include/console_view.h"
#include "include/save_button.h"
#include "include/switcher.h"
#include "include/backtrace.h"

class ConsoleViewListener : public GLViewListener {
 public:
  ConsoleViewListener(ConsoleView* console_view, SaveButton* save_button,
                      Switcher* draw_grid_switcher,
                      Switcher* colorizer_switcher,
                      Backtrace* backtrace);

  virtual void KeyPressed(uint8_t key, int x, int y);

 private:
  ConsoleView* console_view_;
  SaveButton* save_button_;
  Switcher* draw_grid_switcher_;
  Switcher* colorizer_switcher_;
  Backtrace* backtrace_;
};

#endif  // EDITOR_INCLUDE_CONSOLE_VIEW_LISTENER_H_
