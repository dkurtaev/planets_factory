// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/vertices_toucher.h"

#include <math.h>

#include <set>
#include <utility>
#include <algorithm>
#include <vector>

#include <GL/freeglut.h>

VerticesToucher::VerticesToucher(std::vector<Point3f*>* vertices)
  : vertices_(vertices) {}

void VerticesToucher::MouseFunc(int button, int state, int x, int y) {
  static const float kSelectionMinDistance = 0.1f;

  if (button != GLUT_LEFT_BUTTON || state != 0) {
    return;
  }

  int view[4];
  double model[16];
  double proj[16];
  float z;
  double world[3];
  glGetIntegerv(GL_VIEWPORT, view);
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);

  y = view[3] - y;
  glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  gluUnProject(x, y, z, model, proj, view, world, world + 1, world + 2);

  // Find nearest vertex.
  float min_distance = kSelectionMinDistance;
  Point3f* nearest_vertex = 0;
  unsigned n_vertices = vertices_->size();
  for (unsigned i = 0; i < n_vertices; ++i) {
    Point3f* vertex = vertices_->operator[](i);
    float dist = vertex->SquaredDistanceTo(world[0], world[1], world[2]);

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
