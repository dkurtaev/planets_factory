// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_TRIANGLES_TOUCHER_H_
#define EDITOR_INCLUDE_TRIANGLES_TOUCHER_H_

#include <vector>
#include <utility>

#include "include/glview_listener.h"
#include "include/structures.h"

class TrianglesToucher : public GLViewListener {
 public:
  explicit TrianglesToucher(std::vector<Triangle*>* triangles);

  virtual void MouseFunc(int button, int state, int x, int y);

  virtual void MouseMove(int x, int y);

 protected:
  virtual void DoAction(Triangle* triangle, float bary_p1, float bary_p2,
                        float bary_p3) = 0;

 private:
  void ProcessTouch(int x, int y);

  std::vector<Triangle*>* triangles_;
  bool left_button_pressed_;
};

#endif  // EDITOR_INCLUDE_TRIANGLES_TOUCHER_H_
