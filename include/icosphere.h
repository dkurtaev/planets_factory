#ifndef INCLUDE_ICOSPHERE_H_
#define INCLUDE_ICOSPHERE_H_

#include <vector>

#include "include/structures.h"

class Icosphere {
 public:
  Icosphere(float edge_length);

  ~Icosphere();

  void Draw();

 private:
  // Used for building icosahedron.
  void AddTriangle(unsigned v1, unsigned v2, unsigned v3);

  void SplitTriangles();

  std::vector<Point3f*> vertices_;
  std::vector<Edge*> edges_;
  std::vector<Triangle*> triangles_;

  float* vertices_array_;
  float* normals_array_;
  unsigned short* indices_array_;

  float radius_;
};

#endif  // INCLUDE_ICOSPHERE_H_
