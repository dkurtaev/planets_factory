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
  edges_.reserve(6);
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

void Point3f::AddNeighbor(Point3f* point, Edge* edge) {
  neighborhood_.push_back(point);
  edges_.push_back(edge);
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

void Point3f::GetPosition(int16_t* dst, float* norm) const {
  for (uint8_t i = 0; i < 3; ++i) {
    dst[i] = INT16_MAX * (vertices_array_offset_[i] / norm_);
  }
  *norm = norm_;
}

void Point3f::ResetNeighborhood() {
  neighborhood_.clear();
  edges_.clear();
}

Edge* Point3f::GetEdgeTo(const Point3f* point) const {
  const uint8_t n_neighbors = neighborhood_.size();
  for (uint8_t i = 0; i < n_neighbors; ++i) {
    if (neighborhood_[i] == point) {
      return edges_[i];
    }
  }
  return 0;
}

// Edge ---------------------------------------------------------------------
Edge::Edge(Point3f* p1, Point3f* p2)
  : p1_(p1), p2_(p2), middle_point_(0) {
  p1->AddNeighbor(p2, this);
  p2->AddNeighbor(p1, this);
}

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

float Determinant(float* col_1, float* col_2) {
  return col_1[0] * col_2[1] - col_1[1] * col_2[0];
}

// Triangle --------------------------------------------------------------------
Triangle::Triangle(const Point3f* v1, const Point3f* v2, const Point3f* v3,
                   Edge* e1, Edge* e2, Edge* e3) {
  edges_.resize(3);
  points_.resize(3);
  texture_coordinates_ = new uint16_t[6];
  subtriangles_.reserve(4);
  points_[0] = v1;
  points_[1] = v2;
  points_[2] = v3;
  edges_[0] = e1;
  edges_[1] = e2;
  edges_[2] = e3;
}

Triangle::~Triangle() {
  delete[] texture_coordinates_;
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

void Triangle::GetTexCoords(float* dst) const {
  static const float kNormRatio = 1.0f / UINT16_MAX;
  for (uint8_t i = 0; i < 6; ++i) {
    dst[i] = texture_coordinates_[i] * kNormRatio;
  }
}

void Triangle::AddSubtriangle(Triangle* subtriangle) {
  subtriangles_.push_back(subtriangle);
}

void Triangle::GetSubtriangles(std::vector<Triangle*>* subtriangles) {
  const uint8_t n_subtriangles = subtriangles_.size();
  subtriangles->resize(n_subtriangles);
  if (n_subtriangles != 0) {
    std::copy(subtriangles_.begin(), subtriangles_.end(),
              subtriangles->begin());
  }
}

void Triangle::GetMiddlePointsTexCoords(uint16_t* dst) const {
  // Texture_coordinates_: p1u, p1v, p2u, p2v, p3u, p3v.
  for (uint8_t i = 0; i < 3; ++i) {
    uint8_t from = i * 2;
    uint8_t to = ((i + 1) % 3) * 2;
    dst[from] = texture_coordinates_[from] / 2 +
                 texture_coordinates_[to] / 2;
    dst[from + 1] = texture_coordinates_[from + 1] / 2 +
                     texture_coordinates_[to + 1] / 2;
  }
}

bool Triangle::IsInsideCone(float x, float y, float z, float* bary_p1,
                            float* bary_p2, float* bary_p3) {
  // Solve linear system
  // px = a*p1x + b*p2x + c*p3x
  // py = a*p1y + b*p2y + c*p3y
  // py = a*p1z + b*p2z + c*p3z
  // if a, b, c in [0, 1] - point inside cone.
  float cols[4][3];
  cols[0][0] = x;
  cols[0][1] = y;
  cols[0][2] = z;
  for (uint8_t i = 0; i < 3; ++i) {
    points_[i]->GetPosition(cols[i + 1]);
  }
  float denominator = Determinant(cols[1], cols[2], cols[3]);

  *bary_p1 = Determinant(cols[0], cols[2], cols[3]) / denominator;
  if (*bary_p1 < 0.0f || 1.0f < *bary_p1) return false;

  *bary_p2 = Determinant(cols[1], cols[0], cols[3]) / denominator;
  if (*bary_p2 < 0.0f || 1.0f < *bary_p2) return false;

  *bary_p3 = Determinant(cols[1], cols[2], cols[0]) / denominator;
  return (0.0f <= *bary_p3 && *bary_p3 <= 1.0f);
}

void Triangle::GetNormal(int8_t* dst) {
  float points[3][3];
  float normal[3];
  for (uint8_t i = 0; i < 3; ++i) {
    points_[i]->GetPosition(points[i]);
    normal[i] = 0.0f;
  }

  uint8_t idx_1, idx_2;
  for (uint8_t i = 0; i < 3; ++i) {
    idx_1 = (1 + i) % 3;
    idx_2 = (2 + i) % 3;
    normal[0] += points[i][1] * (points[idx_1][2] - points[idx_2][2]);
    normal[1] += points[i][0] * (points[idx_2][2] - points[idx_1][2]);
    normal[2] += points[i][0] * (points[idx_1][1] - points[idx_2][1]);
  }

  float norm = 0;
  for (uint8_t i = 0; i < 3; ++i) {
    norm += normal[i] * normal[i];
  }
  norm = sqrt(norm);
  for (uint8_t i = 0; i < 3; ++i) {
    dst[i] = INT8_MAX * (normal[i] / norm);
  }
}
