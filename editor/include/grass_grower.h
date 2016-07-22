// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_GRASS_GROWER_H_
#define EDITOR_INCLUDE_GRASS_GROWER_H_

#include <vector>
#include <map>

#include "include/triangles_toucher.h"
#include "include/grass_field.h"
#include "include/switcher.h"
#include "include/structures.h"
#include "include/backtrace.h"

class GrassGrower : public TrianglesToucher {
 public:
  GrassGrower(GrassField* grass_field, std::vector<Triangle*>* triangles,
              Backtrace* backtrace, Switcher* is_enabled_swither);

 private:
  static const uint8_t kMaxNumGrassObjects = 3;

  virtual void DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                        float bary_p3);

  virtual void InitAction();

  virtual void FlushAction(Backtrace* backtrace);

  GrassField* grass_field_;
  // Number of grass objects on base triangles.
  std::map<Triangle*, uint8_t> grass_log_;
  unsigned n_last_added_objects_;
};

class GrassGrowerAction : public Action {
 public:
  GrassGrowerAction(GrassField* grass_field, unsigned n_last_added_objects);

  virtual void Undo();

 private:
  GrassField* grass_field_;
  unsigned n_last_added_objects_;
};

#endif  // EDITOR_INCLUDE_GRASS_GROWER_H_
