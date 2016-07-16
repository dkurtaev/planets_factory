// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/console_view_listener.h"

#include <string>

#define GLUT_KEY_BACKSPACE 8
#define GLUT_KEY_ENTER 13

ConsoleViewListener::ConsoleViewListener(ConsoleView* console_view)
  : console_view_(console_view), command_("") {
  console_view_->AddListener(this);
}

void ConsoleViewListener::KeyPressed(uint8_t key, int x, int y) {
  std::string str = console_view_->GetText();
  switch (key) {
    case GLUT_KEY_BACKSPACE: {
      str = str.substr(0, str.length() - 1);
      break;
    }
    case GLUT_KEY_ENTER: {
      command_ = str;
      str = "";
      break;
    }
    default: {
      str += key;
      break;
    }
  }
  console_view_->SetText(str);
}

std::string ConsoleViewListener::ProcessCommand() {
  if (command_ == "") {
    return "";
  } else {
    std::string command_cpy = command_;
    command_ = "";
    return command_cpy;
  }
}
