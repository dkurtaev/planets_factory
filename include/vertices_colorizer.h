#ifndef INCLUDE_VERTICES_COLORIZER_H_
#define INCLUDE_VERTICES_COLORIZER_H_

#include <vector>
#include <utility>

#include "include/vertices_toucher.h"
#include "include/structures.h"

class VerticesColorizer : public VerticesToucher {
 public:
  explicit VerticesColorizer(std::vector<Point3f*>* vertices);

 private:
  virtual void DoAction(std::vector<std::pair<int, Point3f*> >* area);
};

#endif  // INCLUDE_VERTICES_COLORIZER_H_
