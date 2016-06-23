#include "include/vertices_colorizer.h"

#include <vector>
#include <utility>

VerticesColorizer::VerticesColorizer(std::vector<Point3f*>* vertices)
  : VerticesToucher(vertices) {}

void VerticesColorizer::DoAction(std::vector<std::pair<int, Point3f*> >* area) {
  const unsigned n_vertices = area->size();
  for (unsigned i = 0; i < n_vertices; ++i) {
    area->operator[](i).second->SetColor(0, 240, 0);
  }
}
