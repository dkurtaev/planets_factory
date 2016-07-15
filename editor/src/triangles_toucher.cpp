// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/triangles_toucher.h"

#include <vector>

TrianglesToucher::TrianglesToucher(std::vector<Triangle*>* triangles)
  : triangles_(triangles) {}

void TrianglesToucher::ProcessTouch(float x, float y, float z) {
  // Find triangle.
  const unsigned n_tris = triangles_->size();
  Triangle* triangle;
  float bary_p1, bary_p2, bary_p3;
  for (unsigned i = 0; i < n_tris; ++i) {
    triangle = triangles_->operator[](i);
    if (triangle->IsIncludes(x, y, z, &bary_p1, &bary_p2, &bary_p3)) {
      DoAction(triangle, bary_p1, bary_p2, bary_p3);
      break;
    }
  }
}
