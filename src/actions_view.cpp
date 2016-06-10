#include "include/actions_view.h"

#include <vector>
#include <string>

#include <GL/freeglut.h>

ActionsView::ActionsView(const std::vector<GLViewListener*>& actions,
                         const std::vector<std::string>& actions_titles)
  : GLView(100, 50, "Actions"), actions_titles_(actions_titles) {
  const unsigned n_actions = actions.size();
  const float roi_height = 1.0f / n_actions;
  for (unsigned i = 0; i < n_actions; ++i) {
    AddListener(actions[i], Roi(i * roi_height, 0, roi_height, 1));    
  }
}

void ActionsView::Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, display_width_, 0, display_height_, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  const unsigned n_actions = actions_titles_.size();
  const float block_height = static_cast<float>(display_height_) / n_actions;
  for (unsigned i = 0; i < n_actions; ++i) {
    // Block.
    if (i % 2) {
      glColor3f(1.0f, 1.0f, 1.0f);
    } else {
      glColor3f(0.86f, 1.0f, 0.98f);
    }
    glBegin(GL_QUADS);
      glVertex2i(0, i * block_height);
      glVertex2i(display_width_, i * block_height);
      glVertex2i(display_width_, (i + 1) * block_height);
      glVertex2i(0, (i + 1) * block_height);
    glEnd();

    // Text.
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(0, (i + 0.5) * block_height);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
      reinterpret_cast<const unsigned char*>(actions_titles_[n_actions - i - 1].c_str()));
  }

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}
