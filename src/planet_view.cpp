#include "include/planet_view.h"

#include <GL/freeglut.h>

PlanetView::PlanetView()
  : GLView(500, 500, "Planets factory") {
  camera_cs_ = new SphericalCS(20, 0, 80, 0, &planet_cs_);
  camera_ = new Camera(camera_cs_);
  icosphere = new Icosphere(4);
  camera_mover_ = new CameraMover(camera_cs_);
  AddListener(camera_mover_);
  InitGL();
}

PlanetView::~PlanetView() {
  delete camera_mover_;
  delete icosphere;
  delete camera_;
  delete camera_cs_;
}

void PlanetView::Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera_->Setup(display_width_, display_height_);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  icosphere->Draw();

  glBegin(GL_LINES);
  for (int i = 0; i < 3; ++i) {
    glColor3f(i == 0, i == 1, i == 2);
    glVertex3f(0, 0, 0);
    glVertex3f(i == 0 ? 100 : 0, i == 1 ? 100 : 0, i == 2 ? 100 : 0);
  }
  glEnd();

  glutSwapBuffers();
}

void PlanetView::InitGL() {
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
