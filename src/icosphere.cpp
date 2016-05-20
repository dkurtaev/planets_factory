#include "include/icosphere.h"

#include <math.h>
#include <algorithm>

#include <GL/freeglut.h>

Icosphere::Icosphere(float midradius) {
  static const int kNumVertices = 12;
  static const int kNumTris = 20;

  vertices_ = new float[3 * kNumVertices];
  normals_ = new float[3 * kNumTris];
  indices_ = new unsigned short[3 * kNumTris];

  float base_plane_height = midradius * sqrt(6);
  float base_plane_width = 2 * base_plane_height / sqrt(2);

  base_plane_height /= 2;
  base_plane_width /= 2;

  // First plane.
  SetPoint(base_plane_width, -base_plane_height, 0, vertices_);
  SetPoint(base_plane_width, base_plane_height, 0, vertices_ + 3);
  SetPoint(-base_plane_width, base_plane_height, 0, vertices_ + 6);
  SetPoint(-base_plane_width, -base_plane_height, 0, vertices_ + 9);

  // Second plane.
  SetPoint(0, base_plane_width, -base_plane_height, vertices_ + 12);
  SetPoint(0, base_plane_width, base_plane_height, vertices_ + 15);
  SetPoint(0, -base_plane_width, base_plane_height, vertices_ + 18);
  SetPoint(0, -base_plane_width, -base_plane_height, vertices_ + 21);

  // Third plane.
  SetPoint(-base_plane_height, 0, base_plane_width, vertices_ + 24);
  SetPoint(base_plane_height, 0, base_plane_width, vertices_ + 27);
  SetPoint(base_plane_height, 0, -base_plane_width, vertices_ + 30);
  SetPoint(-base_plane_height, 0, -base_plane_width, vertices_ + 33);

  SetTriangle(0, 5, 9, 1);
  SetTriangle(1, 5, 1, 4);
  SetTriangle(2, 5, 4, 2);
  SetTriangle(3, 5, 2, 8);
  SetTriangle(4, 5, 8, 9);

  SetTriangle(5, 0, 1, 9);
  SetTriangle(6, 0, 9, 6);
  SetTriangle(7, 0, 6, 7);
  SetTriangle(8, 0, 7, 10);
  SetTriangle(9, 0, 10, 1);

  SetTriangle(10, 11, 2, 4);
  SetTriangle(11, 11, 4, 10);
  SetTriangle(12, 11, 10, 7);
  SetTriangle(13, 11, 7, 3);
  SetTriangle(14, 11, 3, 2);

  SetTriangle(15, 4, 1, 10);
  SetTriangle(16, 6, 9, 8);
  SetTriangle(17, 6, 8, 3);
  SetTriangle(18, 3, 7, 6);
  SetTriangle(19, 3, 8, 2);
}

Icosphere::~Icosphere() {
  delete[] vertices_;
  delete[] normals_;
  delete[] indices_;
}

void Icosphere::SetPoint(float x, float y, float z, float* dst) {
  dst[0] = x;
  dst[1] = y;
  dst[2] = z;
}

void Icosphere::SetTriangle(int tr_idx, int vert_1, int vert_2, int vert_3) {
  int verts[3];
  float xs[3], ys[3], zs[3];
  verts[0] = vert_1;
  verts[1] = vert_2;
  verts[2] = vert_3;

  for (int i = 0; i < 3; ++i) {
    indices_[tr_idx * 3 + i] = verts[i];
    xs[i] = vertices_[verts[i] * 3];
    ys[i] = vertices_[verts[i] * 3 + 1];
    zs[i] = vertices_[verts[i] * 3 + 2];
  }
  normals_[tr_idx * 3] = ys[0] * (zs[2] - zs[3]) +
                         ys[1] * (zs[3] - zs[1]) +
                         ys[2] * (zs[1] - zs[2]);
  normals_[tr_idx * 3 + 1] = xs[0] * (zs[3] - zs[2]) +
                             xs[1] * (zs[1] - zs[3]) +
                             xs[2] * (zs[2] - zs[1]);
  normals_[tr_idx * 3 + 2] = xs[0] * (ys[2] - ys[3]) +
                             xs[1] * (ys[3] - ys[1]) +
                             xs[2] * (ys[1] - ys[2]);
}

void Icosphere::Draw() {
  glColor3f(0, 0.8, 0);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normals_);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices_);
  glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, indices_);
}
