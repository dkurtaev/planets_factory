// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_ICOSPHERE_H_
#define INCLUDE_ICOSPHERE_H_

#include <stdint.h>

#include <vector>

#include "include/structures.h"

class Icosphere {
 public:
  explicit Icosphere(float radius);

  ~Icosphere();

  // Drawing textured mesh.
  void Draw() const;

  // Drawing icosphere grid.
  void DrawGrid() const;

  void GetVertices(std::vector<Point3f*>* vertices);

  void GetTriangles(std::vector<Triangle*>* triangles);

 private:
  // Setup texture coordinates for icosahedon.
  void SetTexCoords();

  // Used for building icosphere from icosahedron.
  void AddTriangle(unsigned v1, unsigned v2, unsigned v3);

  // Used for building icosphere from icosahedron.
  void SplitTriangles();

  std::vector<Point3f*> vertices_;
  std::vector<Edge*> edges_;
  std::vector<Triangle*> triangles_;

  float* vertices_array_;
  int8_t* normals_array_;
  uint16_t* indices_array_;
  uint16_t* tex_coord_array_;
  // Using for drawing icosphere grid. One color for internal vertices, other
  // color for vertices on initial edges.
  uint8_t* colors_array_;

  float radius_;
};

#endif  // INCLUDE_ICOSPHERE_H_
