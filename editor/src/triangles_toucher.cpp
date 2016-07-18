// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/triangles_toucher.h"

#include <vector>

TrianglesToucher::TrianglesToucher(std::vector<Triangle*>* triangles,
                                   Backtrace* backtrace)
  : Toucher(backtrace), triangles_(triangles) {}

void TrianglesToucher::ProcessTouch(float x, float y, float z) {
  // Find triangle.
  std::vector<Triangle*> subtriangles(*triangles_);
  Triangle* triangle;
  float bary_p1, bary_p2, bary_p3;
  unsigned n_tris;
  bool found;
  do {
    found = false;
    n_tris = subtriangles.size();
    for (unsigned i = 0; i < n_tris; ++i) {
      triangle = subtriangles[i];
      if (triangle->IsInsideCone(x, y, z, &bary_p1, &bary_p2, &bary_p3)) {
        triangle->GetSubtriangles(&subtriangles);
        found = true;
        break;
      }
    }
  } while (!subtriangles.empty() && found);
  if (found) {
    DoAction(triangle, bary_p1, bary_p2, bary_p3);
  }
}
