#ifndef INCLUDE_VERTICES_TOUCHER_H_
#define INCLUDE_VERTICES_TOUCHER_H_

#include <vector>
#include <utility>

#include "include/glview_listener.h"
#include "include/structures.h"

class VerticesToucher : public GLViewListener {
 public:
  explicit VerticesToucher(std::vector<Point3f*>* vertices);

  virtual void MouseFunc(int button, int state, int x, int y);

 protected:
  virtual void DoAction(std::vector<std::pair<int, Point3f*> >* area) = 0;

 private:
  static const unsigned kAreaRadius = 4;

  std::vector<Point3f*>* vertices_;
};

#endif  // INCLUDE_VERTICES_TOUCHER_H_