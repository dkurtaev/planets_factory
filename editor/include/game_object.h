// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_GAME_OBJECT_H_
#define EDITOR_INCLUDE_GAME_OBJECT_H_

#include "include/structures.h"

class GameObject {
 public:
  GameObject(const Triangle* base_triangle) : base_triangle_(base_triangle) {}

  virtual void Draw() = 0;

 protected:
  const Triangle* base_triangle_;
};

#endif  // EDITOR_INCLUDE_GAME_OBJECT_H_
