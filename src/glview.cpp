#include "include/glview.h"

#include <vector>
#include <string>

#include <GL/freeglut.h>

#define FOREACH_LISTENER_IN_ROI \
  GLView* active_view = GetActiveGLView(); \
  const unsigned n_listeners = active_view->listeners_.size(); \
  const float float_x = static_cast<float>(x) / active_view->display_width_; \
  const float float_y = static_cast<float>(y) / active_view->display_height_; \
  for (unsigned i = 0; i < n_listeners; ++i) { \
    if (active_view->listeners_rois_[i].IsIncludes(float_x, float_y)) { \
      active_view->listeners_[i]-> \

#define END_FOREACH ;}}

std::vector<GLView*> GLView::inherited_views_;

GLView::GLView(int display_width, int display_height, std::string window_header)
  : display_width_(display_width), display_height_(display_height) {
  InitWindow(window_header);
  inherited_views_.push_back(this);
}

void GLView::InitWindow(std::string window_header) {
  if (inherited_views_.empty()) {
    int tmp = 0;
    glutInit(&tmp, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
  }
  glutInitWindowSize(display_width_, display_height_);
  glutInitWindowPosition(0, 0);
  window_handle_ = glutCreateWindow(window_header.c_str());
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  glutDisplayFunc(IdleDisplay);
  glutIdleFunc(IdleDisplay);
  glutReshapeFunc(Reshape);
  glutSpecialFunc(SpecialKeyPressed);
  glutMotionFunc(MouseMove);
  glutMouseFunc(MouseFunc);
  glutSpecialUpFunc(SpecialKeyReleased);
  glutCloseFunc(CloseFunc);
}

void GLView::AddListener(GLViewListener* listener, const Roi& roi) {
  listeners_.push_back(listener);
  listeners_rois_.push_back(roi);
}

void GLView::Reshape(int width, int height) {
  GLView* active_view = GetActiveGLView();
  const unsigned n_listeners = active_view->listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    active_view->listeners_[i]->Reshape(width, height);
  }
  active_view->display_width_ = width;
  active_view->display_height_ = height;
  glViewport(0, 0, width, height);
}

void GLView::SpecialKeyPressed(int key, int x, int y) {
  FOREACH_LISTENER_IN_ROI SpecialKeyPressed(key, x, y) END_FOREACH
}

void GLView::SpecialKeyReleased(int key, int x, int y) {
  FOREACH_LISTENER_IN_ROI SpecialKeyReleased(key, x, y) END_FOREACH
}

void GLView::MouseFunc(int button, int state, int x, int y) {
  FOREACH_LISTENER_IN_ROI MouseFunc(button, state, x, y) END_FOREACH
}

void GLView::MouseMove(int x, int y) {
  FOREACH_LISTENER_IN_ROI MouseMove(x, y) END_FOREACH
}

void GLView::IdleDisplay() {
  const unsigned n_views = inherited_views_.size();
  for (unsigned i = 0; i < n_views; ++i) {
    GLView* view = inherited_views_[i];
    const unsigned n_listeners = view->listeners_.size();
    for (unsigned i = 0; i < n_listeners; ++i) {
      view->listeners_[i]->DoEvents();
    }
    glutSetWindow(view->window_handle_);
    view->Display();
  }
}

GLView* GLView::GetActiveGLView() {
  const int handle = glutGetWindow();
  const unsigned size = inherited_views_.size();
  for (unsigned i = 0; i < size; ++i) {
    if (inherited_views_[i]->window_handle_ == handle) {
      return inherited_views_[i];
    }
  }
  return 0;
}

void GLView::CloseFunc() {
  const int handle = glutGetWindow();
  std::vector<GLView*>::iterator it = inherited_views_.begin();
  for (it; it != inherited_views_.end(); ++it) {
    if ((*it)->window_handle_ == handle) {
      inherited_views_.erase(it);
      break;
    }
  }
}

bool Roi::IsIncludes(float x, float y) {
  return (x >= left && x <= left + width &&
          y >= top && y <= top + height);
}
