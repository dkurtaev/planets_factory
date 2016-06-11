#include "include/actions_view.h"

#include <vector>
#include <string>

#include <GL/freeglut.h>

ActionsView::ActionsView(const std::vector<GLViewListener*>& actions,
                         const std::vector<std::string>& actions_titles)
  : GLView(kInitButtonRoiWidth, kInitButtonRoiHeight * actions.size(),
           "Actions"),
    actions_titles_(actions_titles) {
  const unsigned n_actions = actions.size();
  const float roi_height = 1.0f / n_actions;
  const float button_height = roi_height - kBottomIdent - kTopIdent;
  const float button_width = 1 - kRightIdent - kLeftIdent;
  for (unsigned i = 0; i < n_actions; ++i) {
    AddListener(actions[i], Roi(i * roi_height + kTopIdent, kLeftIdent,
                                button_height, button_width));
  }
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

  const unsigned n_actions = actions_titles_.size();
  const float font_height = static_cast<float>(kFontHeight) / display_height_;
  for (unsigned i = 0; i < n_actions; ++i) {
    Roi roi = listeners_rois_[i];

    // Button.
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
      glVertex2f(roi.left, roi.top);
      glVertex2f(roi.left, roi.top + roi.height);
      glVertex2f(roi.left + roi.width, roi.top + roi.height);
      glVertex2f(roi.left + roi.width, roi.top);
    glEnd();

    // Text.
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(roi.left, roi.top + (roi.height + font_height) / 2);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
      reinterpret_cast<const unsigned char*>(actions_titles_[i].c_str()));
  }

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}
