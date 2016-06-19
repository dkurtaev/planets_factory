#include "include/button.h"

Button::Button(std::string text)
  : text_(text) {}

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
  glColor3f(0.8, 0.5, 1);
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
  glColor3f(0, 0, 0);
  glRasterPos2i(-std::min(semiwidth, bmp_length / 2), bmp_height / 2);
  glutBitmapString(GLUT_BITMAP_9_BY_15, text);
}
