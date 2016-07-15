// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_CAMERA_MOVER_H_
#define EDITOR_INCLUDE_CAMERA_MOVER_H_

#include <sys/time.h>

#include "include/glview_listener.h"
#include "include/spherical_cs.h"
#include "include/switcher.h"

class CameraMover : public GLViewListener {
 public:
  explicit CameraMover(SphericalCS* camera_cs);

  virtual void MouseFunc(int button, int state, int x, int y);

  virtual void MouseMove(int x, int y);

  virtual void SpecialKeyPressed(int key, int x, int y);

  virtual void SpecialKeyReleased(int key, int x, int y);

  virtual void DoEvents();

 private:
  static const float kRotationDelta = 8.0f;
  static const float kRotationAroundNormalDelta = 25.0f;
  static const float kMovementAcceleration = -1.0f;
  // If time between last mouse moving and releasing less than constant,
  // acceleration is activating. (time in milliseconds).
  static const float kAccelerationDelay = 20.0f;
  // If speed dx*dx+dy*dy less than constant, inertial movement deactivating.
  static const float kMinimalSpeed = 1e-2f;
  // Radius incrementing value if wheel used.
  static const float kRadiusIncrement = 0.7f;

  static float TimeFrom(const timeval& tv);

  void MoveCamera();

  SphericalCS* camera_cs_;
  bool ctrl_key_pressed_;
  bool left_button_pressed_;
  float last_mouse_x_;
  float last_mouse_y_;

  // Inertial movement.
  bool is_inertial_moving_;
  float init_dx_;
  float init_dy_;
  timeval init_tv_;
  timeval last_mouse_move_;
};

#endif  // EDITOR_INCLUDE_CAMERA_MOVER_H_
