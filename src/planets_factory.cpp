#include <GL/freeglut.h>

#include "include/camera.h"
#include "include/icosphere.h"
#include "include/camera_mover.h"

void InitWindow();

void InitGL();

void Reshape(int width, int height);

void Display();

void SpecialKeyPressed(int key, int x, int y);

void SpecialKeyReleased(int key, int x, int y);

void MouseFunc(int button, int state, int x, int y);

void MouseMove(int x, int y);

int display_width = 500;
int display_height = 500;
int window_handle;

SphericalCS planet_cs;
SphericalCS camera_cs(20, 0, 80, 0, &planet_cs);
Camera* camera;
Icosphere* icosphere;
std::vector<GLViewListener*> glview_listeners;

int main(int argc, char** argv) {
  glview_listeners.push_back(new CameraMover(&camera_cs));
  camera = new Camera(&camera_cs);
  icosphere = new Icosphere(4);

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

  glutDisplayFunc(Display);
  glutIdleFunc(Display);
  glutReshapeFunc(Reshape);
  glutSpecialFunc(SpecialKeyPressed);
  glutMotionFunc(MouseMove);
  glutMouseFunc(MouseFunc);
  glutSpecialUpFunc(SpecialKeyReleased);
}

void InitGL() {
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

void Reshape(int width, int height) {
  const unsigned n_listeners = glview_listeners.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    glview_listeners[i]->Reshape(width, height);
  }
  display_width = width;
  display_height = height;
  glViewport(0, 0, width, height);
}

void Display() {
  const unsigned n_listeners = glview_listeners.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    glview_listeners[i]->DoEvents();
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera->Setup(display_width, display_height);

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

void SpecialKeyPressed(int key, int x, int y) {
  const unsigned n_listeners = glview_listeners.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    glview_listeners[i]->SpecialKeyPressed(key, x, y);
  }
}

void SpecialKeyReleased(int key, int x, int y) {
  const unsigned n_listeners = glview_listeners.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    glview_listeners[i]->SpecialKeyReleased(key, x, y);
  }
}

void MouseFunc(int button, int state, int x, int y) {
  const unsigned n_listeners = glview_listeners.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    glview_listeners[i]->MouseFunc(button, state, x, y);
  }
}

void MouseMove(int x, int y) {
  const unsigned n_listeners = glview_listeners.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    glview_listeners[i]->MouseMove(x, y);
  }
}
