// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_GAME_OBJECT_H_
#define EDITOR_INCLUDE_GAME_OBJECT_H_

#include "include/structures.h"

class GameObject {
 public:
  GameObject(const Triangle* base_triangle);

  void Display();

 protected:
  virtual void DrawObject() = 0;

 private:
  const Triangle* base_triangle_;
};

#endif  // EDITOR_INCLUDE_GAME_OBJECT_H_
