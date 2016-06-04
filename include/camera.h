#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

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
  static const float kRotationDelta = 20;
  static const float kRotationAroundNormalDelta = 10;

  float* model_matrix_;
  SphericalCS* camera_cs_;

  float display_width_;
  float display_height_;
  float last_mouse_x_;
  float last_mouse_y_;
  bool left_button_pressed_;
  bool ctrl_key_pressed_;
};

#endif  // INCLUDE_CAMERA_H_
