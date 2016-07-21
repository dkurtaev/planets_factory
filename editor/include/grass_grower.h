// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_GRASS_GROWER_H_
#define EDITOR_INCLUDE_GRASS_GROWER_H_

#include <vector>

#include "include/triangles_toucher.h"
#include "include/grass_field.h"
#include "include/switcher.h"
#include "include/structures.h"

class GrassGrower : public TrianglesToucher {
 public:
  GrassGrower(GrassField* grass_field, std::vector<Triangle*>* triangles,
              Backtrace* backtrace, Switcher* is_enabled_swither);

 private:
  virtual void DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                        float bary_p3);

  virtual void InitAction() {}

  virtual void FlushAction(Backtrace* backtrace) {}

  GrassField* grass_field_;
};

#endif  // EDITOR_INCLUDE_GRASS_GROWER_H_
