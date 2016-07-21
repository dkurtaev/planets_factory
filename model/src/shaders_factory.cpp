// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/shaders_factory.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <glog/logging.h>

std::set<unsigned> ShadersFactory::created_programs_;

unsigned ShadersFactory::GetProgramFromFile(
    const std::vector<std::string>& vert_shader_paths,
    const std::vector<std::string>& frag_shader_paths) {
  std::ifstream file;
  const uint8_t n_vert_shader_paths = vert_shader_paths.size();
  const uint8_t n_frag_shader_paths = frag_shader_paths.size();
  std::vector<std::string> vert_shader_src(n_vert_shader_paths);
  std::vector<std::string> frag_shader_src(n_frag_shader_paths);

  for (uint8_t i = 0; i < n_vert_shader_paths; ++i) {
    file.open(vert_shader_paths[i].c_str());
    CHECK(file.is_open())
        << "Vertex shader \"" << vert_shader_paths[i] << "\" not found";
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    vert_shader_src[i] = ss.str();
  }
  for (uint8_t i = 0; i < n_frag_shader_paths; ++i) {
    file.open(frag_shader_paths[i].c_str());
    CHECK(file.is_open())
        << "Fragment shader \"" << frag_shader_paths[i] << "\" not found";
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    frag_shader_src[i] = ss.str();
  }
  return GetProgramFromSource(vert_shader_src, frag_shader_src);
}

unsigned ShadersFactory::GetProgramFromSource(
    const std::vector<std::string>& vert_shader_src,
    const std::vector<std::string>& frag_shader_src) {
  unsigned program = glCreateProgram();
  CHECK(program != 0) << "Program creating failed";
  CHECK(created_programs_.find(program) == created_programs_.end())
      << "Not unique program id";
  created_programs_.insert(program);

  unsigned vert_shader = CreateShader(GL_VERTEX_SHADER, vert_shader_src);
  unsigned frag_shader = CreateShader(GL_FRAGMENT_SHADER, frag_shader_src);

  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);

  glBindAttribLocation(program, COORDS_ATTRIB, "a_position");
  glBindAttribLocation(program, COLORS_ATTRIB, "a_color");
  glBindAttribLocation(program, NORMALS_ATTRIB, "a_normal");
  glBindAttribLocation(program, TEX_ATTRIB, "a_tex_coord");

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

  return program;
}

unsigned ShadersFactory::CreateShader(GLenum type,
                                      const std::vector<std::string>& src) {
  unsigned shader = glCreateShader(type);
  CHECK(shader != 0) << "Shader creating failed";

  const uint8_t n_src = src.size();
  const char** src_array = new const char*[n_src];
  for (uint8_t i = 0; i < n_src; ++i) {
    src_array[i] = src[i].c_str();
  }
  glShaderSource(shader, n_src, src_array, 0);
  delete[] src_array;

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
