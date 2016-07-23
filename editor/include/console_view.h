// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_CONSOLE_VIEW_H_
#define EDITOR_INCLUDE_CONSOLE_VIEW_H_

#include <stdint.h>
#include <sys/time.h>

#include <string>
#include <vector>

#include "include/glview.h"
#include "include/glview_listener.h"
#include "include/console_view.h"
#include "include/save_button.h"
#include "include/switcher.h"
#include "include/backtrace.h"

class ConsoleView : public GLView {
 public:
  explicit ConsoleView(GLView* parent);

  virtual void Display();

  void SetText(const std::string& text);

  // Add new command.
  void SetCommand(const std::string& command);

  // If command not satisfied for user, returns value to last_command_
  // (without adding to display queue).
  void ReturnCommand(const std::string& command);

  // Add command to displaying queue.
  void Write(const std::string& text);

  void GetText(std::string* text);

  void Clear();

  // Returns last command (text line after pressing Enter key is a command).
  // Set last command as processed.
  void ProcessCommand(std::string* command);

 private:
  static const uint8_t kFontColor[];
  // Number of commands for displaying.
  static const uint8_t kDisplayLogDepth = 3;
  // Number of milliseconds between redisplaying view.
  static const unsigned kDisplayDelay = 1000;

  std::string text_line_;
  // Last command is empty if processed and not empty if not processed.
  std::string last_command_;
  std::vector<std::string> commands_log_;
  GLView* parent_;
  // If true, need to redisplay.
  bool content_changed_;
  // If content not changes, display by timer.
  timeval last_display_;
};

class ConsoleViewListener : public GLViewListener {
 public:
  ConsoleViewListener(ConsoleView* console_view, SaveButton* save_button,
                      Switcher* draw_grid_switcher,
                      Switcher* colorizer_switcher, Backtrace* backtrace);

  virtual void KeyPressed(uint8_t key, int x, int y);

 private:
  ConsoleView* console_view_;
  SaveButton* save_button_;
  Switcher* draw_grid_switcher_;
  Switcher* colorizer_switcher_;
  Backtrace* backtrace_;
};

#endif  // EDITOR_INCLUDE_CONSOLE_VIEW_H_
