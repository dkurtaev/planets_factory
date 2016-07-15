// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_VERTICES_MOVER_H_
#define EDITOR_INCLUDE_VERTICES_MOVER_H_

#include <vector>
#include <utility>

#include "include/vertices_toucher.h"
#include "include/structures.h"
#include "include/switcher.h"

class VerticesMover : public VerticesToucher {
 public:
  explicit VerticesMover(std::vector<Point3f*>* vertices,
                         Switcher* is_enabled_swither);

 private:
  virtual void DoAction(std::vector<std::pair<int, Point3f*> >* area);
};

#endif  // EDITOR_INCLUDE_VERTICES_MOVER_H_
