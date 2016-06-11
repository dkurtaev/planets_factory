#include "include/camera_mover.h"

#include <math.h>

#include <GL/freeglut.h>

CameraMover::CameraMover(SphericalCS* camera_cs)
  : camera_cs_(camera_cs), left_button_pressed_(false),
    ctrl_key_pressed_(false), is_inertial_moving_(false) {}

void CameraMover::MouseFunc(int button, int state, int x, int y) {
  switch (button) {
    case GLUT_LEFT_BUTTON: {
      left_button_pressed_ = !state;
      last_mouse_x_ = x;
      last_mouse_y_ = y;
      if (!ctrl_key_pressed_) {
        if (left_button_pressed_) {
          is_inertial_moving_ = false;
        } else {
          if (TimeFrom(last_mouse_move_) <= kAccelerationDelay) {
            is_inertial_moving_ = true;
          }
        }
      }
      break;
    }
    case GLUT_WHEEL_UP: {
      float radius = camera_cs_->GetRadius();
      if (radius > kRadiusIncrement) {
        camera_cs_->SetRadius(radius - kRadiusIncrement);
      }
      break;
    }
    case GLUT_WHEEL_DOWN: {
      camera_cs_->SetRadius(camera_cs_->GetRadius() + kRadiusIncrement);
      break;
    }
    default: break;
  }
}

void CameraMover::MouseMove(int x, int y) {
  if (left_button_pressed_) {
    if (!ctrl_key_pressed_) {
      gettimeofday(&init_tv_, 0);
      init_dx_ = (x - last_mouse_x_) / display_width_;
      init_dy_ = (y - last_mouse_y_) / display_height_;
      init_dx_ = (init_dx_ >= 0 ? sqrt(init_dx_) : -sqrt(-init_dx_));
      init_dy_ = (init_dy_ >= 0 ? sqrt(init_dy_) : -sqrt(-init_dy_));
      MoveCamera();
    } else {
      float n_x = x - display_width_ / 2;
      float n_y = -y + display_height_ / 2;
      float n_last_x = last_mouse_x_ - display_width_ / 2;
      float n_last_y = -last_mouse_y_ + display_height_ / 2;
      // Angle between two vectors begins in center of display and ends in
      // previous and new mouse points.
      float a = atan2(n_x * n_last_y - n_last_x * n_y,
                      n_x * n_last_x + n_y * n_last_y);
      // Normalized distance from center of display to new mouse point.
      float ratio = 0.5 * sqrt(pow(n_x / display_width_, 2) +
                               pow(n_y / display_height_, 2));
      camera_cs_->Rotate(SphericalCS::NORMAL,
                         kRotationAroundNormalDelta * a * sqrt(ratio));
    }
    last_mouse_x_ = x;
    last_mouse_y_ = y;
  }

  gettimeofday(&last_mouse_move_, 0);
}

void CameraMover::SpecialKeyPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
      camera_cs_->Rotate(SphericalCS::ORDINATE, -kRotationDelta); break;
    case GLUT_KEY_DOWN:
      camera_cs_->Rotate(SphericalCS::ORDINATE, kRotationDelta); break;
    case GLUT_KEY_LEFT:
      camera_cs_->Rotate(SphericalCS::ABSCISSA, kRotationDelta); break;
    case GLUT_KEY_RIGHT:
      camera_cs_->Rotate(SphericalCS::ABSCISSA, -kRotationDelta); break;
    case GLUT_KEY_CTRL_L: case GLUT_KEY_CTRL_R:
      ctrl_key_pressed_ = true;
      break;
    default: break;
  }
}

void CameraMover::SpecialKeyReleased(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_CTRL_L: case GLUT_KEY_CTRL_R:
      ctrl_key_pressed_ = false;
      break;
    default: break;
  }
}

void CameraMover::MoveCamera() {
  float exp_acc_t = exp(kMovementAcceleration * TimeFrom(init_tv_) * 1e-3);
  float dx = kRotationDelta * init_dx_ * exp_acc_t;
  float dy = -kRotationDelta * init_dy_ * exp_acc_t;

  camera_cs_->Rotate(SphericalCS::ABSCISSA, dx);
  camera_cs_->Rotate(SphericalCS::ORDINATE, dy);
  if (is_inertial_moving_ && dx * dx + dy * dy < kMinimalSpeed) {
    is_inertial_moving_ = false;
  }
}

float CameraMover::TimeFrom(const timeval& tv) {
  timeval current_tv;
  gettimeofday(&current_tv, 0);
  return ((current_tv.tv_sec - tv.tv_sec) * 1e+3 +
          (current_tv.tv_usec - tv.tv_usec) * 1e-3);
}

void CameraMover::DoEvents() {
  if (is_inertial_moving_) {
    MoveCamera();
  }
}
