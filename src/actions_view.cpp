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
    Roi roi(kLeftIdent, 1 - kRightIdent, i * roi_height + kTopIdent,
            (i + 1) * roi_height - kBottomIdent);
    layout_.AddListener(buttons[i], roi);
    buttons_rois_.push_back(roi);
  }
  AddListener(&layout_);
}

void ActionsView::Display() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, display_width_, display_height_, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

  const unsigned n_buttons = buttons_.size();
  unsigned top, left, right, botttom;
  for (unsigned i = 0; i < n_buttons; ++i) {
    glClear(GL_STENCIL_BUFFER_BIT);

    buttons_rois_[i].Get(display_width_, display_height_, &left, &right, &top,
                         &botttom);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glBegin(GL_QUADS);
      glVertex2i(left, top);
      glVertex2i(left, botttom);
      glVertex2i(right, botttom);
      glVertex2i(right, top);
    glEnd();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glPushMatrix();
    glTranslatef((left + right) / 2, (top + botttom) / 2, 0);
    buttons_[i]->Display((right - left) / 2, (botttom - top) / 2);
    glPopMatrix();
  }
  glDisable(GL_STENCIL_TEST);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}
