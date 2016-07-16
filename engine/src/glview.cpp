// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/glview.h"

#include <vector>
#include <string>

#include <GL/freeglut.h>

#define FOREACH_LISTENER \
  GLView* active_view = GetActiveGLView(); \
  const unsigned n_listeners = active_view->listeners_.size(); \
  for (unsigned i = 0; i < n_listeners; ++i) \
    if (active_view->listeners_[i]->IsEnabled()) \
      active_view->listeners_[i]->

std::vector<GLView*> GLView::inherited_views_;

GLView::GLView(int display_width, int display_height, std::string window_header,
               GLView* parent, int sub_x, int sub_y)
  : display_width_(display_width), display_height_(display_height),
    root_(false) {
  InitWindow(window_header, parent, sub_x, sub_y);
  inherited_views_.push_back(this);
}

GLView::~GLView() {
  std::vector<GLView*>::iterator it = inherited_views_.begin();
  for (it; it != inherited_views_.end(); ++it) {
    if ((*it)->window_handle_ == window_handle_) {
      inherited_views_.erase(it);
      glutDestroyWindow(window_handle_);
      break;
    }
  }
}

void GLView::InitWindow(std::string window_header, GLView* parent, int sub_x,
                        int sub_y) {
  if (parent) {
    window_handle_ = glutCreateSubWindow(parent->window_handle_, sub_x, sub_y,
                                         display_width_, display_height_);
  } else {
    glutInitWindowSize(display_width_, display_height_);
    glutInitWindowPosition(0, 0);
    window_handle_ = glutCreateWindow(window_header.c_str());
  }
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  glutDisplayFunc(IdleDisplay);
  glutIdleFunc(IdleDisplay);
  glutReshapeFunc(Reshape);
  glutSpecialFunc(SpecialKeyPressed);
  glutMotionFunc(MouseMove);
  glutPassiveMotionFunc(PassiveMouseMove);
  glutMouseFunc(MouseFunc);
  glutSpecialUpFunc(SpecialKeyReleased);
  glutKeyboardFunc(KeyPressed);
  glutCloseFunc(CloseFunc);
  glutEntryFunc(EntryFunc);
}

void GLView::AddListener(GLViewListener* listener) {
  listeners_.push_back(listener);
}

void GLView::Reshape(int width, int height) {
  FOREACH_LISTENER Reshape(width, height);
  active_view->display_width_ = width;
  active_view->display_height_ = height;
  glViewport(0, 0, width, height);
}

void GLView::SpecialKeyPressed(int key, int x, int y) {
  FOREACH_LISTENER SpecialKeyPressed(key, x, y);
}

void GLView::SpecialKeyReleased(int key, int x, int y) {
  FOREACH_LISTENER SpecialKeyReleased(key, x, y);
}

void GLView::KeyPressed(uint8_t key, int x, int y) {
  FOREACH_LISTENER KeyPressed(key, x, y);
}

void GLView::MouseFunc(int button, int state, int x, int y) {
  FOREACH_LISTENER MouseFunc(button, state, x, y);
}

void GLView::MouseMove(int x, int y) {
  FOREACH_LISTENER MouseMove(x, y);
}

void GLView::PassiveMouseMove(int x, int y) {
  FOREACH_LISTENER PassiveMouseMove(x, y);
}

void GLView::IdleDisplay() {
  const unsigned n_views = inherited_views_.size();
  for (unsigned i = 0; i < n_views; ++i) {
    GLView* view = inherited_views_[i];
    const unsigned n_listeners = view->listeners_.size();
    for (unsigned i = 0; i < n_listeners; ++i) {
      if (view->listeners_[i]->IsEnabled()) {
        view->listeners_[i]->DoEvents();
      }
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
  GLView* view;
  std::vector<GLView*>::iterator it;
  for (it = inherited_views_.begin(); it != inherited_views_.end(); ++it) {
    view = *it;
    if (view->window_handle_ == handle) {
      inherited_views_.erase(it);
      break;
    }
  }
  if (view->root_) {
    for (it = inherited_views_.begin(); it != inherited_views_.end(); ++it) {
      glutDestroyWindow((*it)->window_handle_);
    }
    inherited_views_.clear();
  }
}

void GLView::EntryFunc(int state) {
  FOREACH_LISTENER EntryFunc(state);
}

void GLView::InitGLContext() {
  int tmp = 0;
  glutInit(&tmp, 0);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA |
                      GLUT_STENCIL);
  glutInitWindowSize(0, 0);
  glutInitWindowPosition(0, 0);
  glutDestroyWindow(glutCreateWindow(""));
}

int GLView::GetHeight() const {
  return display_height_;
}

int GLView::GetWidth() const {
  return display_width_;
}

void GLView::Hide() {
  glutSetWindow(window_handle_);
  glutHideWindow();
}

void GLView::AsRootView() {
  root_ = true;
}
