// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/button.h"

#include <string>
#include <algorithm>

#include <GL/freeglut.h>

Button::Button(std::string text)
  : text_(text) {
  font_color_[0] = font_color_[1] = font_color_[2] = 0;
  button_color_[0] = 204;
  button_color_[1] = 127;
  button_color_[2] = 255;
}

void Button::MouseMove(int x, int y) {
  glutSetCursor(GLUT_CURSOR_INFO);
}

void Button::PassiveMouseMove(int x, int y) {
  glutSetCursor(GLUT_CURSOR_INFO);
}

void Button::EntryFunc(int state) {
  if (state == GLUT_LEFT) {
    glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
  }
}

void Button::Display(int semiwidth, int semiheight) {
  // Button.
  glColor3ubv(button_color_);
  glBegin(GL_QUADS);
    glVertex2i(-semiwidth, semiheight);
    glVertex2i(semiwidth, semiheight);
    glVertex2i(semiwidth, -semiheight);
    glVertex2i(-semiwidth, -semiheight);
  glEnd();

  // Text.
  const uint8_t* text = reinterpret_cast<const uint8_t*>(text_.c_str());
  const int bmp_length = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
  const int bmp_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
  glColor3ubv(font_color_);
  glRasterPos2i(-std::min(semiwidth, bmp_length / 2), bmp_height / 2);
  glutBitmapString(GLUT_BITMAP_9_BY_15, text);
}
