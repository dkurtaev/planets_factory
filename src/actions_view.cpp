#include "include/actions_view.h"

#include <vector>
#include <string>

#include <GL/freeglut.h>

ActionsView::ActionsView(std::vector<Button*> buttons, GLView* parent,
                         int sub_x, int sub_y)
  : GLView(kInitButtonRoiWidth, kInitButtonRoiHeight * buttons.size(),
           "Actions", parent, sub_x, sub_y),
    buttons_(buttons) {
  const unsigned n_buttons = buttons.size();
  const float roi_height = 1.0f / n_buttons;
  for (unsigned i = 0; i < n_buttons; ++i) {
    layout_.AddListener(buttons[i], Roi(kLeftIdent, 1 - kRightIdent,
                                        i * roi_height + kTopIdent,
                                        (i + 1) * roi_height - kBottomIdent));
  }
  AddListener(&layout_);
}

void ActionsView::Display() {
  static const unsigned kFontHeight = 10;

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, 1, 1, 0, 0, 1);  // Replacing top and bottom.

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  const float font_height = static_cast<float>(kFontHeight) / display_height_;

  const unsigned n_buttons = buttons_.size();
  const float roi_height = 1.0f / n_buttons;

  for (unsigned i = 0; i < n_buttons; ++i) {
    // Button.
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
      glVertex2f(kLeftIdent, i * roi_height + kTopIdent);
      glVertex2f(kLeftIdent, (i + 1) * roi_height - kBottomIdent);
      glVertex2f(1 - kRightIdent, (i + 1) * roi_height - kBottomIdent);
      glVertex2f(1 - kRightIdent, i * roi_height + kTopIdent);
    glEnd();

    // // Text.
    // glColor3f(0.0f, 0.0f, 0.0f);
    // glRasterPos2f(roi.left, roi.top + (roi.height + font_height) / 2);
    // glutBitmapString(GLUT_BITMAP_HELVETICA_18,
    //   reinterpret_cast<const unsigned char*>(actions_titles_[i].c_str()));
  }

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}
