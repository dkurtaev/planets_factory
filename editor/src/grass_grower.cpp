// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/grass_grower.h"

GrassGrower::GrassGrower(GrassField* grass_field,
                         std::vector<Triangle*>* triangles,
                         Backtrace* backtrace, Switcher* is_enabled_swither)
  : TrianglesToucher(triangles, backtrace), grass_field_(grass_field) {
  is_enabled_swither->SetFlag(&is_enabled_);
}

void GrassGrower::DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                           float bary_p3) {
  grass_field_->AddGrassObject(triangle);
}
