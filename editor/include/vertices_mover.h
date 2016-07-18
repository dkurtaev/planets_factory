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

class VerticesMover : public VerticesToucher {
 public:
  VerticesMover(std::vector<Point3f*>* vertices, Switcher* is_move_up_swither,
                Switcher* is_move_down_swither, Backtrace* backtrace);

  virtual bool IsEnabled();

 private:
  static const unsigned kAreaRadius = 4;

  virtual void DoAction(Point3f* vertex);

  virtual void InitAction();

  virtual void FlushAction(Backtrace* backtrace);

  // Flags for moving vertices up / down by normal.
  bool is_move_up_;
  bool is_move_down_;
  std::map<Point3f*, float> action_data_;
};

class VerticesMoverAction : public Action {
 public:
  explicit VerticesMoverAction(const std::map<Point3f*, float>& data);

  virtual void Undo();

 private:
  std::map<Point3f*, float> data_;
};

#endif  // EDITOR_INCLUDE_VERTICES_MOVER_H_
