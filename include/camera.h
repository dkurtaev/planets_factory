#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

#include <sys/time.h>

#include "include/spherical_cs.h"

class Camera {
 public:
  Camera(SphericalCS* camera_cs);

  ~Camera();

  void Setup(int display_width, int display_height);

  void MouseFunc(int button, int state, int x, int y);

  void MouseMove(int x, int y);

  void SpecialKeyPressed(int key, int x, int y);

  void SpecialKeyReleased(int key, int x, int y);

 private:
  static const float kRotationDelta = 8;
  static const float kRotationAroundNormalDelta = 10;
  static const float kMovementAcceleration = -1;
  // If time between last mouse moving and releasing less than constant,
  // acceleration is activating. (time in milliseconds).
  static const float kAccelerationDelay = 20;

  void MoveCamera();

  static float TimeFrom(const timeval& tv);

  static float TimeBetween(const timeval& tv1, const timeval& tv2);

  float* model_matrix_;
  SphericalCS* camera_cs_;

  float display_width_;
  float display_height_;
  float last_mouse_x_;
  float last_mouse_y_;
  bool left_button_pressed_;
  bool ctrl_key_pressed_;

  // Inertial movement.
  bool is_inertial_moving_;
  timeval last_mouse_move_;

  float init_dx_;
  float init_dy_;
  timeval init_tv_;
};

#endif  // INCLUDE_CAMERA_H_
