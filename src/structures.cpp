// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/structures.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <algorithm>
#include <vector>

// Point3f ---------------------------------------------------------------------
Point3f::Point3f(uint16_t id, float x, float y, float z,
                 float* vertices_array_offset, uint8_t* colors_array_offset)
  : id_(id), vertices_array_offset_(vertices_array_offset),
    colors_array_offset_(colors_array_offset) {
  SetPosition(x, y, z);
  neighborhood_.reserve(6);  // Maximal numer of neighbors.
}

void Point3f::Normalize(float target_norm) {
  float coeff = target_norm / norm_;
  vertices_array_offset_[0] *= coeff;
  vertices_array_offset_[1] *= coeff;
  vertices_array_offset_[2] *= coeff;
  norm_ = target_norm;
}

void Point3f::SetPosition(float x, float y, float z) {
  vertices_array_offset_[0] = x;
  vertices_array_offset_[1] = y;
  vertices_array_offset_[2] = z;
  norm_ = sqrt(x * x + y * y + z * z);
}

void Point3f::SetColor(uint8_t r, uint8_t g, uint8_t b) {
  colors_array_offset_[0] = r;
  colors_array_offset_[1] = g;
  colors_array_offset_[2] = b;
}

void Point3f::SetColor(const uint8_t* src) {
  memcpy(colors_array_offset_, src, sizeof(uint8_t) * 3);
}

void Point3f::MiddlePoint(const Point3f& p1, const Point3f& p2,
                          Point3f* middle_point) {
  middle_point->SetPosition(
    0.5f * (p1.vertices_array_offset_[0] + p2.vertices_array_offset_[0]),
    0.5f * (p1.vertices_array_offset_[1] + p2.vertices_array_offset_[1]),
    0.5f * (p1.vertices_array_offset_[2] + p2.vertices_array_offset_[2]));
}

float Point3f::SquaredDistanceTo(float x, float y, float z) {
  return pow(vertices_array_offset_[0] - x, 2) +
         pow(vertices_array_offset_[1] - y, 2) +
         pow(vertices_array_offset_[2] - z, 2);
}

void Point3f::AddNeighbor(Point3f* point) {
  neighborhood_.push_back(point);
}

void Point3f::GetNeighborhood(std::vector<Point3f*>* neighborhood) {
  neighborhood->resize(neighborhood_.size());
  std::copy(neighborhood_.begin(), neighborhood_.end(), neighborhood->begin());
}

void Point3f::GetPosition(float* x, float* y, float* z) const {
  *x = vertices_array_offset_[0];
  *y = vertices_array_offset_[1];
  *z = vertices_array_offset_[2];
}

void Point3f::GetPosition(float* dst) const {
  memcpy(dst, vertices_array_offset_, sizeof(float) * 3);
}

// Edge ---------------------------------------------------------------------
Edge::Edge(Point3f* p1, Point3f* p2)
  : p1_(p1), p2_(p2), middle_point_(0) {}

const Point3f* Edge::MiddlePoint(Point3f* middle_point) {
  if (middle_point) {
    Point3f::MiddlePoint(*p1_, *p2_, middle_point);
    middle_point_ = middle_point;
  }
  return middle_point_;
}

bool Edge::CompareTo(uint16_t p1_id, uint16_t p2_id) const {
  uint16_t self_p1_id = p1_->GetId();
  uint16_t self_p2_id = p2_->GetId();

  return self_p1_id == p1_id && self_p2_id == p2_id ||
         self_p1_id == p2_id && self_p2_id == p1_id;
}

void Edge::GetPoints(Point3f** p1, Point3f** p2) {
  *p1 = p1_;
  *p2 = p2_;
}

bool Edge::IsInsideEdgeCone(const Point3f& p) {
  // Define p1 as t, p2 as q.
  // Let r - orthogonal to t and q.
  // Check that p = a*t + b*q + c*r
  // where a,b >= 0, c = 0. This gives p inside plane cone based on t and q.
  //      t ^
  //       /
  //      /    p
  //     /
  // r (+)----------> q
  //
  // r = (tz*qy-ty*qz, tx*qz-tz*qx, ty*qx-tx*qy).
  // Solving linear system:
  // tx qx tz*qy-ty*qz | px
  // ty qy tx*qz-tz*qx | py
  // tz qz ty*qx-tx*qy | pz
  static const float kZeroLimit = 1e-4f;

  float px, py, pz, p1x, p1y, p1z, p2x, p2y, p2z;
  p.GetPosition(&px, &py, &pz);
  p1_->GetPosition(&p1x, &p1y, &p1z);
  p2_->GetPosition(&p2x, &p2y, &p2z);

  float cols[][3] = { {p1x, p1y, p1z},
                      {p2x, p2y, p2z},
                      {p1z * p2y - p1y * p2z,
                       p1x * p2z - p1z * p2x,
                       p1y * p2x - p1x * p2y},
                      {px, py, pz}};
  float c = Determinant(cols[0], cols[1], cols[3]);
  if (abs(c) < kZeroLimit) {
    float a = Determinant(cols[3], cols[1], cols[2]);
    float b = Determinant(cols[0], cols[3], cols[2]);
    float denominator = Determinant(cols[0], cols[1], cols[2]);
    if (denominator > 0) {
      return a >= 0 && b >= 0;
    } else {
      return a <= 0 && b <= 0;
    }
  } else {
    return false;
  }
}

float Determinant(float* col_1, float* col_2, float* col_3) {
  return col_1[0] * (col_2[1] * col_3[2] - col_3[1] * col_2[2]) -
         col_2[0] * (col_1[1] * col_3[2] - col_3[1] * col_1[2]) +
         col_3[0] * (col_1[1] * col_2[2] - col_2[1] * col_1[2]);
}

// Triangle --------------------------------------------------------------------
Triangle::Triangle(const Point3f* v1, const Point3f* v2, const Point3f* v3,
                   Edge* e1, Edge* e2, Edge* e3) {
  edges_ = new Edge*[3];
  points_ = new const Point3f*[3];
  texture_coordinates_ = new uint16_t[6];
  points_[0] = v1;
  points_[1] = v2;
  points_[2] = v3;
  edges_[0] = e1;
  edges_[1] = e2;
  edges_[2] = e3;
}

Triangle::~Triangle() {
  delete[] texture_coordinates_;
  delete[] points_;
  delete[] edges_;
}

void Triangle::GetIndices(uint16_t* dst) const {
  for (unsigned i = 0; i < 3; ++i) {
    dst[i] = points_[i]->GetId();
  }
}

void Triangle::GetMiddlePointsIndices(uint16_t* dst) const {
  for (unsigned i = 0; i < 3; ++i) {
    dst[i] = edges_[i]->MiddlePoint()->GetId();
  }
}

void Triangle::SetTexCoords(const uint16_t* src) {
  memcpy(texture_coordinates_, src, sizeof(uint16_t) * 6);
}

void Triangle::GetTexCoords(uint16_t* dst) const {
  memcpy(dst, texture_coordinates_, sizeof(uint16_t) * 6);
}

bool Triangle::IsIncludes(float x, float y, float z, float* bary_p1,
                          float* bary_p2, float* bary_p3) {
  // 1. Check that p is on triangle's plane.
  //  p1 ______ p2      p on triangle's plane if determinant
  //     \ *p /         | px p1x p2x p3x | is null.
  //      \  /          | py p1y p2y p3y |
  //       \/ p3        | pz p1z p2z p3z |
  //                    | 1  1   1   1   |
  static const float kZeroLimit = 1e-4f;

  float cols[4][3];
  cols[0][0] = x;
  cols[0][1] = y;
  cols[0][2] = z;
  for (uint8_t i = 0; i < 3; ++i) {
    points_[i]->GetPosition(cols[i + 1]);
  }

  *bary_p1 = Determinant(cols[0], cols[2], cols[3]);
  float denominator = Determinant(cols[1], cols[2], cols[3]);

  float determinant = -denominator;
  determinant += *bary_p1;
  determinant -= Determinant(cols[0], cols[1], cols[3]);
  determinant += Determinant(cols[0], cols[1], cols[2]);
  if (abs(determinant) < kZeroLimit) {
    // 2. Check that barycentric coordinates in interval [0, 1]. This gives
    //    point p is inside triangle.
    // p = a*p1 + b*p2 + c*p3, where a,b,c - barycentric coordinates.
    *bary_p1 /= denominator;
    if (*bary_p1 < 0.0f || 1.0f < *bary_p1) return false;

    *bary_p2 = Determinant(cols[1], cols[0], cols[3]) / denominator;
    if (*bary_p2 < 0.0f || 1.0f < *bary_p2) return false;

    *bary_p3 = Determinant(cols[1], cols[2], cols[0]) / denominator;
    return (0.0f <= *bary_p3 && *bary_p3 <= 1.0f);
  } else {
    return false;
  }
}
