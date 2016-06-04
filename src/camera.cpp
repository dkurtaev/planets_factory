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
      break;
    }
    default: break;
  }
}

void Camera::MouseMove(int x, int y) {
  static const float kRotationAroundNormalMaxAngle = 30 * M_PI / 180;

  if (left_button_pressed_) {
    if (!ctrl_key_pressed_) {
      float dx = (x - last_mouse_x_) / display_width_;
      float dy = (y - last_mouse_y_) / display_height_;
      camera_cs_->Rotate(SphericalCS::ABSCISSA, kRotationDelta * dx);
      camera_cs_->Rotate(SphericalCS::ORDINATE, -kRotationDelta * dy);
    } else {
      float n_x = x - display_width_ / 2;
      float n_y = -y + display_height_ / 2;
      float n_last_x = last_mouse_x_ - display_width_ / 2;
      float n_last_y = -last_mouse_y_ + display_height_ / 2;
      // Angle between two vectors begins in center of display and ends in
      // previous and new mouse points.
      float a = atan2(n_x * n_last_y - n_last_x * n_y,
                      n_x * n_last_x + n_y * n_last_y);
      if (fabs(a) < kRotationAroundNormalMaxAngle) {
        camera_cs_->Rotate(SphericalCS::NORMAL, kRotationAroundNormalDelta * a);
      }
    }
    last_mouse_x_ = x;
    last_mouse_y_ = y;
  }
}
