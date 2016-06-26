#include "include/vertices_colorizer.h"

#include <vector>
#include <utility>

VerticesColorizer::VerticesColorizer(
    std::vector<Point3f*>* vertices,
    const ChangeColorButton* change_color_button)
  : VerticesToucher(vertices), change_color_button_(change_color_button) {}

void VerticesColorizer::DoAction(std::vector<std::pair<int, Point3f*> >* area) {
  uint8_t color[3];
  change_color_button_->GetSelectedColor(color);

  const unsigned n_vertices = area->size();
  for (unsigned i = 0; i < n_vertices; ++i) {
    area->operator[](i).second->SetColor(color[0], color[1], color[2]);
  }
}
