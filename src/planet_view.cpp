#include "include/planet_view.h"
#include "include/shaders_factory.h"

#include <string>
#include <GL/freeglut.h>

PlanetView::PlanetView(const Icosphere* icosphere, SphericalCS* camera_cs,
             CameraMover* camera_mover, VerticesColorizer* vertices_colorizer)
  : GLView(500, 500, "Planets factory"), icosphere_(icosphere),
    camera_(camera_cs) {
  AddListener(camera_mover);
  AddListener(vertices_colorizer);
  InitGL();
  planet_shader_program_ = ShadersFactory::GetProgramFromFile(
                               "../res/shaders/test_shader.vertex",
                               "../res/shaders/test_shader.fragment");
}

void PlanetView::Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera_.Setup(display_width_, display_height_);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glUseProgram(planet_shader_program_);
  //
  float modelview_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
  unsigned loc = glGetUniformLocation(planet_shader_program_, "modelview_matrix");
  glUniformMatrix4fv(loc, 1, false, modelview_matrix);

  float projection_matrix[16];
  glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);
  loc = glGetUniformLocation(planet_shader_program_, "projection_matrix");
  glUniformMatrix4fv(loc, 1, false, projection_matrix);
  //
  icosphere_->Draw();
  glUseProgram(0);  // Disable shader program.

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
