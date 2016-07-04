#ifndef INCLUDE_SHADERS_FACTORY_H_
#define INCLUDE_SHADERS_FACTORY_H_

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

class ShadersFactory {
 public:
  static unsigned GetProgramFromFile(const char* vert_shader_path,
                                     const char* frag_shader_path);
  
  static unsigned GetProgramFromSource(const char* vert_shader_src,
                                       const char* frag_shader_src);
 
 private:
  static unsigned CreateShader(GLenum type, const char* src);
};

#endif  // INCLUDE_SHADERS_FACTORY_H_
