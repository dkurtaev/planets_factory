// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

#include "include/spherical_cs.h"

class Camera {
 public:
  explicit Camera(SphericalCS* camera_cs);

  ~Camera();

  void Setup(int display_width, int display_height);

 private:
  float* model_matrix_;
  SphericalCS* camera_cs_;
};

#endif  // INCLUDE_CAMERA_H_
