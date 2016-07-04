#include "include/planet_view.h"

#include <string>
#include <GL/freeglut.h>

#include <glog/logging.h>

PlanetView::PlanetView(const Icosphere* icosphere, SphericalCS* camera_cs,
             CameraMover* camera_mover, VerticesColorizer* vertices_colorizer)
  : GLView(500, 500, "Planets factory"), icosphere_(icosphere),
    camera_(camera_cs) {
  AddListener(camera_mover);
  AddListener(vertices_colorizer);
  InitGL();
  CreateShaderProgram();
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

void PlanetView::CreateShaderProgram() {
  const char* vert_shader_src =
     "attribute vec3 a_position;"
     "attribute vec3 a_color;"
     "uniform mat4 modelview_matrix;"
     "uniform mat4 projection_matrix;"
     "varying vec3 v_color;"
     "void main() {"
       "v_color = a_color;"
       "gl_Position = modelview_matrix * vec4(a_position, 1.0);"
       "gl_Position = projection_matrix * gl_Position;"
     "}";
  const char* frag_shader_src =
      "varying vec3 v_color;"
      "void main() {"
      "  gl_FragColor = vec4(v_color, 1.0);"
      "  gl_FragColor[1] = 0.0;"
      "}";

  planet_shader_program_ = glCreateProgram();
  CHECK(planet_shader_program_ != 0);

  GLuint vert_shader = CreateShader(GL_VERTEX_SHADER, vert_shader_src);
  GLuint frag_shader = CreateShader(GL_FRAGMENT_SHADER, frag_shader_src);

  glAttachShader(planet_shader_program_, vert_shader);
  glAttachShader(planet_shader_program_, frag_shader);

  glBindAttribLocation(planet_shader_program_, 0, "a_position");
  glBindAttribLocation(planet_shader_program_, 1, "a_color");

  glLinkProgram(planet_shader_program_);
  GLint success = GL_FALSE;
  glGetProgramiv(planet_shader_program_, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(planet_shader_program_, GL_INFO_LOG_LENGTH, &log_length);
    GLchar* log_msg = new GLchar[log_length];
    glGetProgramInfoLog(planet_shader_program_, log_length, &log_length,
                        log_msg);
    CHECK(success != GL_FALSE) << "Cannot compile shader program: " << log_msg;
  }
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  CHECK(glGetAttribLocation(planet_shader_program_, "a_position") == 0);
  CHECK(glGetAttribLocation(planet_shader_program_, "a_color") == 1);
}

GLuint PlanetView::CreateShader(GLenum type, const char* src) {
  GLuint shader = glCreateShader(type);
  CHECK(shader != 0) << "Shader creating failed";
  glShaderSource(shader, 1, &src, 0);

  glCompileShader(shader);
  GLint success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
      GLint log_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
      GLchar* log_msg = new GLchar[log_length];
      glGetShaderInfoLog(shader, log_length, &log_length, log_msg);
      CHECK(success != GL_FALSE)
          << "Cannot compile "
          << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
          << " shader: " << log_msg;
  }
  return shader;
}
