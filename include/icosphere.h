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

  void Draw() const;

  void GetVertices(std::vector<Point3f*>* vertices);

 private:
  static const uint8_t kInitialColor = 204;

  void SetTexCoords();

  // Used for building.
  void AddTriangle(unsigned v1, unsigned v2, unsigned v3);

  void SplitTriangles();

  std::vector<Point3f*> vertices_;
  std::vector<Edge*> edges_;
  std::vector<Triangle*> triangles_;

  float* vertices_array_;
  int8_t* normals_array_;
  uint8_t* colors_array_;
  uint16_t* indices_array_;
  uint16_t* tex_coord_array_;

  float radius_;
};

#endif  // INCLUDE_ICOSPHERE_H_
