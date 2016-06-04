#include "include/camera.h"

#include <math.h>

#include <GL/freeglut.h>

Camera::Camera(SphericalCS* camera_cs)
  : left_button_pressed_(false), ctrl_key_pressed_(false) {
  model_matrix_ = new float[16];
  camera_cs_ = camera_cs;
}

Camera::~Camera() {
  delete model_matrix_;
}

void Camera::Setup(int display_width, int display_height) {
  static const float kZNear = 0.1f;
  static const float kZFar = 1000.0f;
  static const float kFovy = 30.0f;

  display_height_ = display_height;
  display_width_ = display_width;
  if (is_inertial_moving_) {
    MoveCamera();
  }
  camera_cs_->GetModelMatrix(model_matrix_);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(kFovy, display_width_ / display_height_, kZNear, kZFar);
  gluLookAt(model_matrix_[SphericalCS::POSITION_X],
            model_matrix_[SphericalCS::POSITION_Y],
            model_matrix_[SphericalCS::POSITION_Z],
            0, 0, 0,
            -model_matrix_[SphericalCS::ABSCISSA_X],
            -model_matrix_[SphericalCS::ABSCISSA_Y],
            -model_matrix_[SphericalCS::ABSCISSA_Z]);
}

void Camera::SpecialKeyPressed(int key, int x, int y) {
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

void Camera::SpecialKeyReleased(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_CTRL_L: case GLUT_KEY_CTRL_R:
      ctrl_key_pressed_ = false;
      break;
    default: break;
  }
}

void Camera::MouseFunc(int button, int state, int x, int y) {
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

void Camera::MouseMove(int x, int y) {
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

void Camera::MoveCamera() {
  float exp_acc_t = exp(kMovementAcceleration * TimeFrom(init_tv_) * 1e-3);
  float dx = kRotationDelta * init_dx_ * exp_acc_t;
  float dy = -kRotationDelta * init_dy_ * exp_acc_t;   
  
  camera_cs_->Rotate(SphericalCS::ABSCISSA, dx);
  camera_cs_->Rotate(SphericalCS::ORDINATE, dy);
  if (is_inertial_moving_ && dx * dx + dy * dy < kMinimalSpeed) {
    is_inertial_moving_ = false;
  }
}

float Camera::TimeBetween(const timeval& tv1, const timeval& tv2) {
  return ((tv1.tv_sec - tv2.tv_sec) * 1e+3 +
          (tv1.tv_usec - tv2.tv_usec) * 1e-3);
}

float Camera::TimeFrom(const timeval& tv) {
  timeval current_tv;
  gettimeofday(&current_tv, 0);
  return ((current_tv.tv_sec - tv.tv_sec) * 1e+3 +
          (current_tv.tv_usec - tv.tv_usec) * 1e-3);
}
