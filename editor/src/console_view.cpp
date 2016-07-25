// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/console_view.h"

#include <string>

#include <GL/freeglut.h>

#define GLUT_KEY_BACKSPACE 8
#define GLUT_KEY_ENTER 13
#define GLUT_CTRL_S 19  // Save button click.
#define GLUT_CTRL_G 7  // Switch grid visualization.
#define GLUT_CTRL_C 3  // Switch colorizer.
#define GLUT_CTRL_Z 26  // Undoing changes.

const uint8_t ConsoleView::kFontColor[] = {0, 204, 0};

ConsoleView::ConsoleView(GLView* parent)
  : GLView(1, 1, "", parent), parent_(parent), last_command_(""),
    content_changed_(false) {
  Clear();
}

void ConsoleView::Display() {
  if (!content_changed_ && TimeFrom(last_display_) < kDisplayDelay) {
    return;
  }
  gettimeofday(&last_display_, 0);

  glClearColor(0.19f, 0.04f, 0.34f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, display_width_, display_height_, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glLoadIdentity();

    unsigned bmp_height = 0;
    unsigned bmp_width;
    unsigned font_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
    glColor3ubv(kFontColor);
    // Draw log.
    const uint8_t n_commands = commands_log_.size();
    for (uint8_t i = 0; i < n_commands; ++i) {
      std::string line = "planet@factory:~$ " + commands_log_[i];
      const uint8_t* text = reinterpret_cast<const uint8_t*>(line.c_str());
      bmp_width = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
      glRasterPos2i(0, bmp_height + font_height * 0.8f);
      glutBitmapString(GLUT_BITMAP_9_BY_15, text);
      bmp_height += font_height;
    }
    // Draw text line.
    std::string line = "planet@factory:~$ " + text_line_ + '_';
    const uint8_t* text = reinterpret_cast<const uint8_t*>(line.c_str());
    bmp_width = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
    glRasterPos2i(0, bmp_height + font_height * 0.8f);
    glutBitmapString(GLUT_BITMAP_9_BY_15, text);
    bmp_height += font_height;

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  if (parent_ != 0) {
    glutReshapeWindow(parent_->GetWidth(), bmp_height);
    glutPostRedisplay();
    glutPositionWindow(0, parent_->GetHeight() - bmp_height);
  } else {
    glutReshapeWindow(bmp_width, bmp_height);
    glutPostRedisplay();
  }
  glutSwapBuffers();

  content_changed_= display_height_ != bmp_height;  // For dynamically growing.
}

void ConsoleView::SetText(const std::string& text) {
  text_line_ = text;
  content_changed_ = true;
}

void ConsoleView::SetCommand(const std::string& command) {
  last_command_ = command;
  Write(last_command_);
}

void ConsoleView::ReturnCommand(const std::string& command) {
  last_command_ = command;
}

void ConsoleView::GetText(std::string* text) {
  *text = text_line_;
}

void ConsoleView::Clear() {
  text_line_ = "";
  commands_log_.clear();
  content_changed_ = true;
}

void ConsoleView::ProcessCommand(std::string* command) {
  *command = last_command_;
  last_command_ = "";
}

void ConsoleView::Write(const std::string& text) {
  commands_log_.push_back(text);
  if (commands_log_.size() > kDisplayLogDepth) {
    commands_log_.erase(commands_log_.begin() + commands_log_.size() -
                        kDisplayLogDepth - 1);
  }
  content_changed_ = true;
}

ConsoleViewListener::ConsoleViewListener(ConsoleView* console_view,
                                         SaveButton* save_button,
                                         Switcher* draw_grid_switcher,
                                         Switcher* colorizer_switcher,
                                         Backtrace* backtrace)
  : console_view_(console_view), draw_grid_switcher_(draw_grid_switcher),
    save_button_(save_button), colorizer_switcher_(colorizer_switcher),
    backtrace_(backtrace) {
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
    case GLUT_CTRL_Z: {
      backtrace_->Undo();
      break;
    }
    default: {
      str += key;
      break;
    }
  }
  console_view_->SetText(str);
}
