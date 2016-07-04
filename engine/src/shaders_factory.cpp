#include "include/shaders_factory.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <glog/logging.h>

unsigned ShadersFactory::GetProgramFromFile(const char* vert_shader_path,
                                            const char* frag_shader_path) {
  std::ifstream file(vert_shader_path);
  CHECK(file.is_open())
      << "Vertex shader \"" << vert_shader_path << "\" not found";
  std::stringstream vert_shader_src;
  vert_shader_src << file.rdbuf();
  file.close();

  file.open(frag_shader_path);
  CHECK(file.is_open())
      << "Fragment shader \"" << frag_shader_path << "\" not found";
  std::stringstream frag_shader_src;
  frag_shader_src << file.rdbuf();
  file.close();

  return GetProgramFromSource(vert_shader_src.str().c_str(),
                              frag_shader_src.str().c_str());
}

unsigned ShadersFactory::GetProgramFromSource(const char* vert_shader_src,
                                              const char* frag_shader_src) {
  unsigned program = glCreateProgram();
  CHECK(program != 0);

  unsigned vert_shader = CreateShader(GL_VERTEX_SHADER, vert_shader_src);
  unsigned frag_shader = CreateShader(GL_FRAGMENT_SHADER, frag_shader_src);

  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);

  glBindAttribLocation(program, 0, "a_position");
  glBindAttribLocation(program, 1, "a_color");

  glLinkProgram(program);
  GLint success = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    GLchar* log_msg = new GLchar[log_length];
    glGetProgramInfoLog(program, log_length, &log_length,
                        log_msg);
    CHECK(success != GL_FALSE) << "Cannot compile shader program: " << log_msg;
  }
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  CHECK(glGetAttribLocation(program, "a_position") == 0);
  CHECK(glGetAttribLocation(program, "a_color") == 1);
  return program;
}

unsigned ShadersFactory::CreateShader(GLenum type, const char* src) {
  unsigned shader = glCreateShader(type);
  CHECK(shader != 0) << "Shader creating failed";
  glShaderSource(shader, 1, &src, 0);

  glCompileShader(shader);
  int success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
      int log_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
      char* log_msg = new char[log_length];
      glGetShaderInfoLog(shader, log_length, &log_length, log_msg);
      CHECK(success != GL_FALSE)
          << "Cannot compile "
          << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
          << " shader: " << log_msg;
  }
  return shader;
}
