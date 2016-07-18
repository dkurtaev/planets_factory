// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_VERTICES_TOUCHER_H_
#define EDITOR_INCLUDE_VERTICES_TOUCHER_H_

#include <vector>
#include <utility>

#include "include/toucher.h"
#include "include/structures.h"
#include "include/backtrace.h"

class VerticesToucher : public Toucher {
 public:
  VerticesToucher(std::vector<Point3f*>* vertices, Backtrace* backtrace);

 protected:
  virtual void DoAction(Point3f* vertex) = 0;

 private:
  virtual void ProcessTouch(float x, float y, float z);

  std::vector<Point3f*>* vertices_;
};

#endif  // EDITOR_INCLUDE_VERTICES_TOUCHER_H_
