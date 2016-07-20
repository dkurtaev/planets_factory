// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/game_object.h"

#include <stdlib.h>
#include <string.h>

#include <GL/freeglut.h>

#include "include/spherical_cs.h"

GameObject::GameObject(const Triangle* base_triangle)
  : base_triangle_(base_triangle) {}

void GameObject::Display() {
  float model_matrix[16];
  memset(model_matrix, 0, sizeof(float) * 16);
  for (unsigned i = 0; i < 4; ++i) {
    model_matrix[i * 5] = 1.0f;
  }
  base_triangle_->GetNormal(&model_matrix[SphericalCS::NORMAL]);
  // Let (nx, ny, nz) - normal.
  // Wee need to find to vectors, (ax, ay, az) and (ox, oy, oz), orthogonal to
  // normal and to each other.
  // 1. ax*nx + ay*ny + az*nz = 0 => (ax, ay, az) = (-nz, 0, nx)
  // 2. ox*nx + oy*ny + oz*nz = 0     ox*nx + oy*ny + oz*nz = 0
  //    ox*ax + oy*ay + oz*az = 0 => -ox*nz +       + oz*nx = 0 =>
  //    (ox, oy, oz) = (nx*ny, -nx*nx-nz*nz, ny*nz)
  float nx = model_matrix[SphericalCS::NORMAL_X];
  float ny = model_matrix[SphericalCS::NORMAL_Y];
  float nz = model_matrix[SphericalCS::NORMAL_Z];
  model_matrix[SphericalCS::ABSCISSA_X] = -nz;
  model_matrix[SphericalCS::ABSCISSA_Y] = 0.0f;
  model_matrix[SphericalCS::ABSCISSA_Z] = nx;
  Normalize(&model_matrix[SphericalCS::ABSCISSA]);
  model_matrix[SphericalCS::ORDINATE_X] = nx * ny;
  model_matrix[SphericalCS::ORDINATE_Y] = -nx * nx - nz * nz;
  model_matrix[SphericalCS::ORDINATE_Z] = ny * nz;
  Normalize(&model_matrix[SphericalCS::ORDINATE]);

  float triangle_vertices[9];
  base_triangle_->GetCoords(triangle_vertices);
  for (uint8_t i = 0; i < 3; ++i) {
    model_matrix[SphericalCS::POSITION_X + i] = 0;
    for (uint8_t j = 0; j < 3; ++j) {
      model_matrix[SphericalCS::POSITION_X + i] += 0.33f *
                                                   triangle_vertices[j * 3 + i];
    }
  }

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glMultMatrixf(model_matrix);
    DrawObject();    
  glPopMatrix();
}
