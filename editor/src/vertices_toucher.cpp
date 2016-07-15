// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/vertices_toucher.h"

#include <vector>
#include <cfloat>

VerticesToucher::VerticesToucher(std::vector<Point3f*>* vertices)
  : vertices_(vertices) {}

void VerticesToucher::ProcessTouch(float x, float y, float z) {
  // Find nearest vertex.
  float min_distance = FLT_MAX;
  Point3f* nearest_vertex = 0;
  unsigned n_vertices = vertices_->size();
  for (unsigned i = 0; i < n_vertices; ++i) {
    Point3f* vertex = vertices_->operator[](i);
    float dist = vertex->SquaredDistanceTo(x, y, z);
    if (dist < min_distance) {
      nearest_vertex = vertex;
      min_distance = dist;
    }
  }
  if (nearest_vertex) {
    DoAction(nearest_vertex);
  }
}
