#ifndef INCLUDE_PLANET_H_
#define INCLUDE_PLANET_H_

#include "include/spherical_cs.h"

class Planet {
 public:
  Planet(float radius);

  void Draw();

 private:
  float radius_;
};

#endif  // INCLUDE_PLANET_H_
