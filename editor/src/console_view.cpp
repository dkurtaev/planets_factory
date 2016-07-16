// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/console_view.h"

#include <string>

#include <GL/freeglut.h>

const uint8_t ConsoleView::kFontColor[] = {0, 204, 0};

ConsoleView::ConsoleView(GLView* parent)
  : GLView(1, 1, "", parent), parent_(parent), last_command_("") {
  Clear();
}

void ConsoleView::Display() {
  glClearColor(0.19f, 0.04f, 0.34f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, display_width_, display_height_, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glLoadIdentity();

    int console_height = 0;
    glColor3ubv(kFontColor);
    // Draw log.
    const uint8_t n_commands = commands_log_.size();
    for (uint8_t i = 0; i < n_commands; ++i) {
      std::string line = "planet@factory:~$ " + commands_log_[i];
      const uint8_t* text = reinterpret_cast<const uint8_t*>(line.c_str());
      int bmp_length = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
      int bmp_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
      glRasterPos2i(0, console_height + bmp_height * 0.8f);
      glutBitmapString(GLUT_BITMAP_9_BY_15, text);
      console_height += bmp_height;
    }
    // Draw text line.
    std::string line = "planet@factory:~$ " + text_line_ + '_';
    const uint8_t* text = reinterpret_cast<const uint8_t*>(line.c_str());
    int bmp_length = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
    int bmp_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
    glRasterPos2i(0, console_height + bmp_height * 0.8f);
    glutBitmapString(GLUT_BITMAP_9_BY_15, text);
    console_height += bmp_height;

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  if (parent_ != 0) {
    glutReshapeWindow(parent_->GetWidth(), console_height);
    glutPositionWindow(0, parent_->GetHeight() - console_height);
  } else {
    glutReshapeWindow(bmp_length, console_height);
  }

  glutSwapBuffers();
}

void ConsoleView::SetText(const std::string& text) {
  text_line_ = text;
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
}
