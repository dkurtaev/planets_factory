// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/console_view.h"

#include <string>

#include <GL/freeglut.h>

const uint8_t ConsoleView::kFontColor[] = {0, 204, 0};

ConsoleView::ConsoleView(GLView* parent)
  : GLView(1, 1, "", parent), parent_(parent) {
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

    std::string line = "planet@factory:~$ " + text_line_ + '_';
    const uint8_t* text = reinterpret_cast<const uint8_t*>(line.c_str());
    int bmp_length = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
    int bmp_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
    glColor3ubv(kFontColor);
    glRasterPos2i(0, bmp_height * 0.8f);
    glutBitmapString(GLUT_BITMAP_9_BY_15, text);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  if (parent_ != 0) {
    glutReshapeWindow(parent_->GetWidth(), bmp_height);
    glutPositionWindow(0, parent_->GetHeight() - bmp_height);
  } else {
    glutReshapeWindow(bmp_length, bmp_height);
  }

  glutSwapBuffers();
}

void ConsoleView::SetText(const std::string& text) {
  text_line_ = text;
}

std::string ConsoleView::GetText() {
  return text_line_;
}

void ConsoleView::Clear() {
  text_line_ = "";
}
