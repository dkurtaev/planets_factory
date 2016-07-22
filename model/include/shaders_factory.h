// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef MODEL_INCLUDE_SHADERS_FACTORY_H_
#define MODEL_INCLUDE_SHADERS_FACTORY_H_

#include <string>
#include <vector>
#include <set>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

enum Attrib { COORDS_ATTRIB,   // Vertex coordinates attribute (a_position).
              COLORS_ATTRIB,   // Vertex color attribute (a_color).
              NORMALS_ATTRIB,  // Vertex normal attibute (a_normal).
              TEX_ATTRIB };    // Vertex texture coords attribute (a_tex_coord).

class ShadersFactory {
 public:
  static unsigned GetProgramFromFile(
      const std::vector<std::string>& vert_shader_paths,
      const std::vector<std::string>& frag_shader_paths);

  static unsigned GetProgramFromSource(
      const std::vector<std::string>& vert_shader_src,
      const std::vector<std::string>& frag_shader_src);

 private:
  static unsigned CreateShader(GLenum type,
                               const std::vector<std::string>& src);

  static std::set<unsigned> created_programs_;
};

#endif  // MODEL_INCLUDE_SHADERS_FACTORY_H_
