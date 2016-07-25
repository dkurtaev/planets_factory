// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/shortcuts_listener.h"

#include <string>

#include <GL/freeglut.h>

#define GLUT_KEY_BACKSPACE 8
#define GLUT_KEY_ENTER 13
#define GLUT_CTRL_S 19  // Save button click.
#define GLUT_CTRL_G 7  // Switch grid visualization.
#define GLUT_CTRL_C 3  // Switch colorizer.
#define GLUT_CTRL_Z 26  // Undoing changes.

ShortcutsListener::ShortcutsListener(SaveButton* save_button,
                                       Switcher* draw_grid_switcher,
                                       Switcher* colorizer_switcher,
                                       Backtrace* backtrace)
  : draw_grid_switcher_(draw_grid_switcher), save_button_(save_button),
    colorizer_switcher_(colorizer_switcher), backtrace_(backtrace) {}

void ShortcutsListener::KeyPressed(uint8_t key, int x, int y) {
  switch (key) {
    case GLUT_CTRL_S: save_button_->Save(); break;
    case GLUT_CTRL_G: draw_grid_switcher_->Switch(); break;
    case GLUT_CTRL_C: colorizer_switcher_->Switch(); break;
    case GLUT_CTRL_Z: backtrace_->Undo(); break;
    default: break;
  }
}
