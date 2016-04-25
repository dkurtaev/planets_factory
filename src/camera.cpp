#include "include/camera.h"

#include <math.h>

#include <GL/freeglut.h>

Camera::Camera() {
  model_matrix_ = new float[16];
}

Camera::~Camera() {
  delete model_matrix_;
}

void Camera::Setup() {
  static const float kZNear = 0.1f;
  static const float kZFar = 1000.0f;
  static const float kFovy = 30.0f;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(kFovy, display_width_ / display_height_, kZNear, kZFar);

  camera_cs_.GetModelMatrix(model_matrix_);

  gluLookAt(model_matrix_[SphericalCS::POSITION_X],
            model_matrix_[SphericalCS::POSITION_Y],
            model_matrix_[SphericalCS::POSITION_Z],
            0, 0, 0,
            -model_matrix_[SphericalCS::ABSCISSA_X],
            -model_matrix_[SphericalCS::ABSCISSA_Y],
            -model_matrix_[SphericalCS::ABSCISSA_Z]);
}
