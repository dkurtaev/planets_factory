#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

#include "include/spherical_cs.h"

class Camera {
 public:
  Camera();

  ~Camera();

  void Setup();

 private:
  float display_width_;
  float display_height_;
  float* model_matrix_;
  SphericalCS camera_cs_;
};

#endif  // INCLUDE_CAMERA_H_
