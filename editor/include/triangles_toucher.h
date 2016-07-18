// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_TRIANGLES_TOUCHER_H_
#define EDITOR_INCLUDE_TRIANGLES_TOUCHER_H_

#include <vector>

#include "include/toucher.h"
#include "include/structures.h"
#include "include/backtrace.h"

class TrianglesToucher : public Toucher {
 public:
  TrianglesToucher(std::vector<Triangle*>* triangles, Backtrace* backtrace);

 protected:
  virtual void DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                        float bary_p3) = 0;

 private:
  virtual void ProcessTouch(float x, float y, float z);

  std::vector<Triangle*>* triangles_;
};

#endif  // EDITOR_INCLUDE_TRIANGLES_TOUCHER_H_
