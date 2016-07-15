// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/vertices_toucher.h"

#include <set>
#include <vector>
#include <utility>
#include <algorithm>

VerticesToucher::VerticesToucher(std::vector<Point3f*>* vertices)
  : vertices_(vertices) {}

void VerticesToucher::ProcessTouch(float x, float y, float z) {
  static const float kSelectionMinDistance = 0.1f;

  // Find nearest vertex.
  float min_distance = kSelectionMinDistance;
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
    std::vector<std::pair<int, Point3f*> > area;
    area.push_back(std::pair<int, Point3f*>(0, nearest_vertex));

    std::vector<Point3f*> area_level(1, nearest_vertex);
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
    DoAction(&area);
  }
}
