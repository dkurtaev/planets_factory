#include "include/camera.h"

#include <math.h>

#include <GL/freeglut.h>

Camera::Camera(SphericalCS* camera_cs) {
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

  camera_cs_->GetModelMatrix(model_matrix_);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(kFovy, static_cast<float>(display_width) / display_height,
                 kZNear, kZFar);
  gluLookAt(model_matrix_[SphericalCS::POSITION_X],
            model_matrix_[SphericalCS::POSITION_Y],
            model_matrix_[SphericalCS::POSITION_Z],
            0, 0, 0,
            -model_matrix_[SphericalCS::ABSCISSA_X],
            -model_matrix_[SphericalCS::ABSCISSA_Y],
            -model_matrix_[SphericalCS::ABSCISSA_Z]);
}
