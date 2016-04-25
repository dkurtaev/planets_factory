#include "include/planet.h"

#include <GL/freeglut.h>

Planet::Planet(float radius)
  : radius_(radius) {
}

void Planet::Draw() {
  glColor3f(0.0f, 0.5f, 0.3f);
  glutSolidSphere(radius_, 10, 10);
}
