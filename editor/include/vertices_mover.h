// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_VERTICES_MOVER_H_
#define EDITOR_INCLUDE_VERTICES_MOVER_H_

#include <map>
#include <vector>
#include <utility>

#include "include/vertices_toucher.h"
#include "include/structures.h"
#include "include/switcher.h"
#include "include/backtrace.h"
#include "include/icosphere.h"
#include "include/grass_field.h"

class VerticesMover : public VerticesToucher {
 public:
  VerticesMover(Icosphere* icosphere, Switcher* is_move_up_swither,
                Switcher* is_move_down_swither, Backtrace* backtrace,
                GrassField* grass_field);

  virtual bool IsEnabled();

  // Return angle of drawing cone for highlighting ring.
  float GetHighlightingAngle(uint8_t n_splits) const;

 private:
  static const unsigned kAreaRadius = 4;

  virtual void DoAction(Point3f* vertex);

  virtual void InitAction();

  virtual void FlushAction(Backtrace* backtrace);

  // Flags for moving vertices up / down by normal.
  bool is_move_up_;
  bool is_move_down_;
  std::map<Point3f*, float> action_data_;
  Icosphere* icosphere_;
  GrassField* grass_field_;
};

class VerticesMoverAction : public Action {
 public:
  explicit VerticesMoverAction(const std::map<Point3f*, float>& data);

  virtual void Undo();

 private:
  std::map<Point3f*, float> data_;
};

#endif  // EDITOR_INCLUDE_VERTICES_MOVER_H_
