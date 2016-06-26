#ifndef INCLUDE_VERTICES_COLORIZER_H_
#define INCLUDE_VERTICES_COLORIZER_H_

#include <vector>
#include <utility>

#include "include/vertices_toucher.h"
#include "include/structures.h"
#include "include/change_color_button.h"

class VerticesColorizer : public VerticesToucher {
 public:
  VerticesColorizer(std::vector<Point3f*>* vertices,
                    const ChangeColorButton* change_color_button);

 private:
  virtual void DoAction(std::vector<std::pair<int, Point3f*> >* area);

  const ChangeColorButton* change_color_button_;
};

#endif  // INCLUDE_VERTICES_COLORIZER_H_
