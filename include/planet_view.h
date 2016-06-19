#ifndef INCLUDE_PLANET_VIEW_H_
#define INCLUDE_PLANET_VIEW_H_

#include "include/glview.h"
#include "include/spherical_cs.h"
#include "include/camera.h"
#include "include/camera_mover.h"
#include "include/icosphere.h"
#include "include/vertices_colorizer.h"

class PlanetView : public GLView {
 public:
  PlanetView(const Icosphere* icosphere, SphericalCS* camera_cs,
             CameraMover* camera_mover, VerticesColorizer* vertices_colorizer);

  virtual void Display();

 private:
  void InitGL();

  Camera camera_;
  const Icosphere* icosphere_;
};

#endif  // INCLUDE_PLANET_VIEW_H_
