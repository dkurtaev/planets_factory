// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef MODEL_INCLUDE_SHADERS_FACTORY_H_
#define MODEL_INCLUDE_SHADERS_FACTORY_H_

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

enum Attrib { COORDS_ATTRIB,   // Vertex coordinates attribute (a_position).
              COLORS_ATTRIB,   // Vertex color attribute (a_color).
              NORMALS_ATTRIB,  // Vertex normal attibute (a_normal).
              TEX_ATTRIB };    // Vertex texture coords attribute (a_tex_coord).

class ShadersFactory {
 public:
  static unsigned GetProgramFromFile(const char* vert_shader_path,
                                     const char* frag_shader_path);

  static unsigned GetProgramFromSource(const char* vert_shader_src,
                                       const char* frag_shader_src);

 private:
  static unsigned CreateShader(GLenum type, const char* src);
};

#endif  // MODEL_INCLUDE_SHADERS_FACTORY_H_
