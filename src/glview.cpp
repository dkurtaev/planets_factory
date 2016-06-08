#include "include/glview.h"

#include <vector>
#include <string>

#include <GL/freeglut.h>

std::vector<GLView*> GLView::inherited_views_;

GLView::GLView(int display_width, int display_height, std::string window_header)
  : display_width_(display_width), display_height_(display_height) {
  InitWindow(window_header);
  InitGL();
  inherited_views_.push_back(this);
}

void GLView::InitWindow(std::string window_header) {
  int tmp = 0;
  glutInit(&tmp, 0);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
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
}

void GLView::InitGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  glEnable(GL_LIGHT0);
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1 };
  GLfloat position[] = { 100, 100, 100, 1 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void GLView::AddListener(GLViewListener* listener) {
  listeners_.push_back(listener);
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
  GLView* active_view = GetActiveGLView();
  const unsigned n_listeners = active_view->listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    active_view->listeners_[i]->SpecialKeyPressed(key, x, y);
  }
}

void GLView::SpecialKeyReleased(int key, int x, int y) {
  GLView* active_view = GetActiveGLView();
  const unsigned n_listeners = active_view->listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    active_view->listeners_[i]->SpecialKeyReleased(key, x, y);
  }
}

void GLView::MouseFunc(int button, int state, int x, int y) {
  GLView* active_view = GetActiveGLView();
  const unsigned n_listeners = active_view->listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    active_view->listeners_[i]->MouseFunc(button, state, x, y);
  }
}

void GLView::MouseMove(int x, int y) {
  GLView* active_view = GetActiveGLView();
  const unsigned n_listeners = active_view->listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    active_view->listeners_[i]->MouseMove(x, y);
  }
}

void GLView::IdleDisplay() {
  GLView* active_view = GetActiveGLView();
  const unsigned n_listeners = active_view->listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    active_view->listeners_[i]->DoEvents();
  }
  active_view->Display();
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
