// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_CONSOLE_VIEW_H_
#define EDITOR_INCLUDE_CONSOLE_VIEW_H_

#include <stdint.h>

#include <string>

#include "include/glview.h"

class ConsoleView : public GLView {
 public:
  explicit ConsoleView(GLView* parent);

  virtual void Display();

  void SetText(const std::string& text);

  std::string GetText();

  void Clear();

 private:
  static const uint8_t kFontColor[];

  std::string text_line_;
  GLView* parent_;
};

#endif  // EDITOR_INCLUDE_CONSOLE_VIEW_H_
