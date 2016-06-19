#include "include/button.h"

#include <GL/freeglut.h>

Button::Button(std::string text)
  : text_(text) {
  font_color_[0] = font_color_[1] = font_color_[2] = 0.0f;
  button_color_[0] = 0.8f;
  button_color_[1] = 0.5f;
  button_color_[2] = 1.0f;
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
  glColor3fv(button_color_);
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
  glColor3fv(font_color_);
  glRasterPos2i(-std::min(semiwidth, bmp_length / 2), bmp_height / 2);
  glutBitmapString(GLUT_BITMAP_9_BY_15, text);
}
