#ifndef INCLUDE_PLANET_VIEW_H_
#define INCLUDE_PLANET_VIEW_H_

#include "include/glview.h"
#include "include/spherical_cs.h"
#include "include/camera.h"
#include "include/camera_mover.h"
#include "include/icosphere.h"

class PlanetView : public GLView {
 public:
  PlanetView();

  ~PlanetView();

  virtual void Display();

 private:
  Camera* camera_;
  SphericalCS planet_cs_;
  SphericalCS* camera_cs_;
  CameraMover* camera_mover_;
  Icosphere* icosphere;
};

#endif  // INCLUDE_PLANET_VIEW_H_
