// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/console_view_listener.h"

#include <string>

#define GLUT_KEY_BACKSPACE 8
#define GLUT_KEY_ENTER 13
#define GLUT_CTRL_S 19  // Save button click.
#define GLUT_CTRL_G 7  // Switch grid visualization.
#define GLUT_CTRL_C 3  // Switch colorizer.

ConsoleViewListener::ConsoleViewListener(ConsoleView* console_view,
                                         SaveButton* save_button,
                                         Switcher* draw_grid_switcher,
                                         Switcher* colorizer_switcher)
  : console_view_(console_view), draw_grid_switcher_(draw_grid_switcher),
    save_button_(save_button), colorizer_switcher_(colorizer_switcher) {
  console_view_->AddListener(this);
}

void ConsoleViewListener::KeyPressed(uint8_t key, int x, int y) {
  std::string str;
  console_view_->GetText(&str);
  switch (key) {
    case GLUT_KEY_BACKSPACE: {
      str = str.substr(0, str.length() - 1);
      break;
    }
    case GLUT_KEY_ENTER: {
      console_view_->SetCommand(str);
      str = "";
      break;
    }
    case GLUT_CTRL_S: {
      save_button_->Save();
      break;
    }
    case GLUT_CTRL_G: {
      draw_grid_switcher_->Switch();
      break;
    }
    case GLUT_CTRL_C: {
      colorizer_switcher_->Switch();
      break;
    }
    default: {
      str += key;
      break;
    }
  }
  console_view_->SetText(str);
}
