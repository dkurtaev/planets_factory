// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/vertices_mover.h"

#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include <glog/logging.h>

VerticesMover::VerticesMover(std::vector<Point3f*>* vertices,
                             Switcher* is_move_up_swither,
                             Switcher* is_move_down_swither)
  : VerticesToucher(vertices), is_move_up_(false), is_move_down_(false) {
  is_move_up_swither->SetFlag(&is_move_up_);
  is_move_down_swither->SetFlag(&is_move_down_);
}

void VerticesMover::DoAction(Point3f* vertex) {
  static const float kIncrement = 1e-3f;

  CHECK(!is_move_up_ || !is_move_down_);

  float norm = vertex->GetNorm();
  vertex->Normalize(norm + (is_move_up_ ? kIncrement : -kIncrement));

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
      vertex = *it;
      norm = vertex->GetNorm();
      vertex->Normalize(norm + (is_move_up_ ? kIncrement : -kIncrement));
    }
  }
}

bool VerticesMover::IsEnabled() {
  return is_move_up_ || is_move_down_;
}
