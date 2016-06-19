#ifndef INCLUDE_VERTICES_COLORIZER_H_
#define INCLUDE_VERTICES_COLORIZER_H_

#include <vector>

#include "include/glview_listener.h"
#include "include/structures.h"

class VerticesColorizer : public GLViewListener {
 public:
  VerticesColorizer(std::vector<Point3f*>* vertices);

  virtual void MouseFunc(int button, int state, int x, int y);
 
 private:
  std::vector<Point3f*>* vertices_;
};

#endif  // INCLUDE_VERTICES_COLORIZER_H_