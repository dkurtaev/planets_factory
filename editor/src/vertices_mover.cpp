// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/vertices_mover.h"

#include <vector>
#include <utility>

VerticesMover::VerticesMover(std::vector<Point3f*>* vertices,
                             Switcher* is_enabled_swither)
  : VerticesToucher(vertices) {
  is_enabled_swither->SetFlag(&is_enabled_);
}

void VerticesMover::DoAction(std::vector<std::pair<int, Point3f*> >* area) {
  static const float kIncrement = 1e-3f;

  Point3f* point;
  const unsigned n_vertices = area->size();
  for (unsigned i = 0; i < n_vertices; ++i) {
    point = area->operator[](i).second;
    point->Normalize(point->GetNorm() + kIncrement);
  }
}
