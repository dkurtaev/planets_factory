#include <GL/freeglut.h>

#include "include/camera.h"
#include "include/planet.h"
#include "include/icosphere.h"

#include <iostream>

void InitWindow();

void InitGL();

void Reshape(int width, int height);

void Display();

void SpecialKeyPressed(int key, int x, int y);

int display_width = 500;
int display_height = 500;
int window_handle;

SphericalCS planet_cs;
SphericalCS camera_cs(20, 0, 80, 0, &planet_cs);
Camera* camera;
Planet* planet;
Icosphere* icosphere;

int main(int argc, char** argv) {
  camera = new Camera(&camera_cs);
  planet = new Planet(5);
  icosphere = new Icosphere(5, 2.5);

  InitWindow();
  InitGL();
  glutMainLoop();
  return 0;
}

void InitWindow() {
  int tmp = 0;
  glutInit(&tmp, 0);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
  glutInitWindowSize(display_width, display_height);
  glutInitWindowPosition(0, 0);
  window_handle = glutCreateWindow("Planets factory");
//  glutSetCursor(GLUT_CURSOR_NONE);
//  glutIgnoreKeyRepeat(1);
//  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  glutDisplayFunc(Display);
  glutIdleFunc(Display);
  glutReshapeFunc(Reshape);
//  glutKeyboardFunc(KeyPressed);
//  glutKeyboardUpFunc(KeyReleased);
  glutSpecialFunc(SpecialKeyPressed);
//  glutSpecialUpFunc(SpecialKeyReleased);
//  glutEntryFunc(MouseEntry);
//  glutPassiveMotionFunc(MouseMove);
//  glutMouseFunc(MouseFunc);
}

void InitGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glEnable(GL_NORMALIZE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_LIGHT0);
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1 };
  GLfloat position[] = { 100, 100, 100, 1 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void Reshape(int width, int height) {
  display_width = width;
  display_height = height;
  glViewport(0, 0, width, height);
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera->Setup(display_width, display_height);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
//  planet->Draw();
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

void SpecialKeyPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP: camera_cs.Rotate(SphericalCS::ORDINATE, -5); break;
    case GLUT_KEY_DOWN: camera_cs.Rotate(SphericalCS::ORDINATE, 5); break;
    case GLUT_KEY_LEFT: camera_cs.Rotate(SphericalCS::ABSCISSA, 5); break;
    case GLUT_KEY_RIGHT: camera_cs.Rotate(SphericalCS::ABSCISSA, -5); break;
  }
}
