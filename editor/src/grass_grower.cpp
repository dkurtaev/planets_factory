// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/grass_grower.h"

#include <vector>
#include <map>

GrassGrower::GrassGrower(GrassField* grass_field,
                         std::vector<Triangle*>* triangles,
                         Backtrace* backtrace, Switcher* is_enabled_swither)
  : TrianglesToucher(triangles, backtrace), grass_field_(grass_field),
    n_last_added_objects_(0) {
  is_enabled_swither->SetFlag(&is_enabled_);
}

void GrassGrower::DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                           float bary_p3) {
  std::map<Triangle*, uint8_t>::iterator it = grass_log_.find(triangle);
  if (it == grass_log_.end() || it->second < kMaxNumGrassObjects) {
    grass_field_->AddGrassObject(triangle, bary_p1, bary_p2, bary_p3);
    ++n_last_added_objects_;
    if (it == grass_log_.end()) {
      grass_log_[triangle] = 1;
    } else {
      it->second += 1;
    }
  }
}

void GrassGrower::InitAction() {
  n_last_added_objects_ = 0;
}

void GrassGrower::FlushAction(Backtrace* backtrace) {
  if (n_last_added_objects_) {
    backtrace->AddAction(new GrassGrowerAction(grass_field_,
                                               n_last_added_objects_));
  }
}

GrassGrowerAction::GrassGrowerAction(GrassField* grass_field,
                                     unsigned n_last_added_objects)
  : grass_field_(grass_field), n_last_added_objects_(n_last_added_objects) {}

void GrassGrowerAction::Undo() {
  grass_field_->RemoveLastGrassObjects(n_last_added_objects_);
}
