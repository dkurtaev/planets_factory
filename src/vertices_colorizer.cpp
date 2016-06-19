#include "include/vertices_colorizer.h"

#include <math.h>

#include <GL/freeglut.h>

VerticesColorizer::VerticesColorizer(std::vector<Point3f*>* vertices)
  : vertices_(vertices) {}

void VerticesColorizer::MouseFunc(int button, int state, int x, int y) {
  static const float kSelectionMinDistance = 0.1f;

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
  const unsigned n_vertices = vertices_->size();
  for (unsigned i = 0; i < n_vertices; ++i) {
    Point3f* vertex = (*vertices_)[i];
    float dist = vertex->SquaredDistanceTo(world[0], world[1], world[2]);
    
    if (dist < min_distance) {
      nearest_vertex = vertex;
      min_distance = dist;
    }
  }

  if (nearest_vertex) {
    nearest_vertex->SetColor(0, 240, 0);
  }
}
