// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/spherical_cs.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <GL/freeglut.h>

SphericalCS::SphericalCS(float radius, float azimuth, float zenith,
                         float own_rotation, SphericalCS* center)
  : center_(center), radius_(radius), update_global_matrix_(false),
    global_model_matrix_(0) {
  azimuth *= M_PI / 180;
  zenith *= M_PI / 180;

  float cos_azimuth = cos(azimuth);
  float sin_azimuth = sin(azimuth);
  float cos_zenith = cos(zenith);
  float sin_zenith = sin(zenith);

  // Identity matrix as initial value.
  local_model_matrix_ = new float[16];
  memset(local_model_matrix_, 0, sizeof(float) * 16);
  for (unsigned i = 0; i < 4; ++i) {
    local_model_matrix_[i * 5] = 1.0f;
  }

  local_model_matrix_[NORMAL_X] = sin_azimuth * cos_zenith;
  local_model_matrix_[NORMAL_Y] = sin_zenith;
  local_model_matrix_[NORMAL_Z] = cos_azimuth * cos_zenith;

  local_model_matrix_[ABSCISSA_X] = sin_azimuth * sin_zenith;
  local_model_matrix_[ABSCISSA_Y] = -cos_zenith;
  local_model_matrix_[ABSCISSA_Z] = cos_azimuth * sin_zenith;

  local_model_matrix_[ORDINATE_X] = cos_azimuth;
  local_model_matrix_[ORDINATE_Y] = 0;
  local_model_matrix_[ORDINATE_Z] = -sin_azimuth;

  local_model_matrix_[POSITION_X] = radius_ * local_model_matrix_[NORMAL_X];
  local_model_matrix_[POSITION_Y] = radius_ * local_model_matrix_[NORMAL_Y];
  local_model_matrix_[POSITION_Z] = radius_ * local_model_matrix_[NORMAL_Z];

  Rotate(NORMAL, own_rotation);

  if (center_ != 0) {
    global_model_matrix_ = new float[16];
    update_global_matrix_ = true;
  }
}

SphericalCS::~SphericalCS() {
  delete[] local_model_matrix_;
  delete[] global_model_matrix_;
}

void SphericalCS::Rotate(unsigned axis_id, float angle) {
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  glLoadMatrixf(local_model_matrix_);
  switch (axis_id) {
    case ORDINATE: glRotatef(angle, 1, 0, 0); break;
    case NORMAL:   glRotatef(angle, 0, 1, 0); break;
    case ABSCISSA: glRotatef(angle, 0, 0, 1); break;
    default: break;
  }
  glGetFloatv(GL_MODELVIEW_MATRIX, local_model_matrix_);
  local_model_matrix_[POSITION_X] = radius_ * local_model_matrix_[NORMAL_X];
  local_model_matrix_[POSITION_Y] = radius_ * local_model_matrix_[NORMAL_Y];
  local_model_matrix_[POSITION_Z] = radius_ * local_model_matrix_[NORMAL_Z];
  if (center_ != 0) {
    update_global_matrix_ = true;
  }

  glPopMatrix();
}

void SphericalCS::GetModelMatrix(float* matrix) {
  if (center_ != 0) {
    if (GlobalMatrixNeedsToUpdate()) {
      center_->GetModelMatrix(matrix);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadMatrixf(matrix);
      glMultMatrixf(local_model_matrix_);
      glGetFloatv(GL_MODELVIEW_MATRIX, global_model_matrix_);
      glPopMatrix();

      update_global_matrix_ = false;
    }
    memcpy(matrix, global_model_matrix_, sizeof(float) * 16);
  } else {
    memcpy(matrix, local_model_matrix_, sizeof(float) * 16);
  }
}

bool SphericalCS::GlobalMatrixNeedsToUpdate() {
  return update_global_matrix_ ||
      center_ != 0 && center_->GlobalMatrixNeedsToUpdate();
}

void SphericalCS::SetRadius(float radius) {
  float ratio = radius / radius_;
  local_model_matrix_[POSITION_X] *= ratio;
  local_model_matrix_[POSITION_Y] *= ratio;
  local_model_matrix_[POSITION_Z] *= ratio;
  radius_ = radius;
  update_global_matrix_ = true;
}

float SphericalCS::GetRadius() const {
  return radius_;
}
