#include "include/structures.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <algorithm>
#include <vector>

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

Triangle::Triangle(const Point3f* v1, const Point3f* v2, const Point3f* v3,
                   Edge* e1, Edge* e2, Edge* e3) {
  points_[0] = v1;
  points_[1] = v2;
  points_[2] = v3;
  edges_[0] = e1;
  edges_[1] = e2;
  edges_[2] = e3;
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
