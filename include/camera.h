#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

#ifndef GLUT_WHEEL_UP
  #define GLUT_WHEEL_UP 3
#endif

#ifndef GLUT_WHEEL_DOWN
  #define GLUT_WHEEL_DOWN 4
#endif

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
