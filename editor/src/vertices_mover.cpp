// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/vertices_mover.h"

#include <set>
#include <vector>
#include <utility>
#include <algorithm>

VerticesMover::VerticesMover(std::vector<Point3f*>* vertices,
                             Switcher* is_enabled_swither)
  : VerticesToucher(vertices) {
  is_enabled_swither->SetFlag(&is_enabled_);
}

void VerticesMover::DoAction(Point3f* vertex) {
  static const float kIncrement = 1e-3f;

  std::vector<std::pair<int, Point3f*> > area;
  area.push_back(std::pair<int, Point3f*>(0, vertex));

  std::vector<Point3f*> area_level(1, vertex);
  std::vector<Point3f*> neighborhood;
  std::set<Point3f*> unique_neighbors;
  std::vector<Point3f*>::iterator it;

  for (unsigned i = 1; i <= kAreaRadius; ++i) {
    for (it = area_level.begin(); it != area_level.end(); ++it) {
      (*it)->GetNeighborhood(&neighborhood);
      unique_neighbors.insert(neighborhood.begin(), neighborhood.end());
    }

    area_level.resize(unique_neighbors.size());
    std::copy(unique_neighbors.begin(), unique_neighbors.end(),
              area_level.begin());
    unique_neighbors.clear();

    for (it = area_level.begin(); it != area_level.end(); ++it) {
      area.push_back(std::pair<int, Point3f*>(i, *it));
    }
  }

  const unsigned n_vertices = area.size();
  for (unsigned i = 0; i < n_vertices; ++i) {
    vertex = area[i].second;
    vertex->Normalize(vertex->GetNorm() + kIncrement);
  }
}
