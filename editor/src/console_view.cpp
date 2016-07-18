// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/console_view.h"

#include <string>

#include <GL/freeglut.h>

const uint8_t ConsoleView::kFontColor[] = {0, 204, 0};

ConsoleView::ConsoleView(GLView* parent)
  : GLView(1, 1, "", parent), parent_(parent), last_command_(""),
    content_changed_(false) {
  Clear();
}

void ConsoleView::Display() {
  if (!content_changed_) {
    timeval now;
    gettimeofday(&now, 0);
    if ((now.tv_sec - last_display_.tv_sec) * 1e+3 +
        (now.tv_usec - last_display_.tv_usec) * 1e-3 < kDisplayDelay) {
      return;
    }
    last_display_ = now;
  }

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
    glutPositionWindow(0, parent_->GetHeight() - bmp_height);
  } else {
    glutReshapeWindow(bmp_width, bmp_height);
  }
  glutPostRedisplay();
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
