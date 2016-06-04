#include "include/structures.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

Point3f::Point3f(float x, float y, float z, uint16_t id)
  : id(id) {
  data[0] = x;
  data[1] = y;
  data[2] = z;
}

void Point3f::Normalize(float target_norm) {
  float norm = 0;
  for (unsigned i = 0; i < 3; ++i) {
    norm += pow(data[i], 2);
  }
  norm = target_norm / sqrt(norm);
  for (unsigned i = 0; i < 3; ++i) {
    data[i] *= norm;
  }
}

void Point3f::GetCoordinates(float* dst) const {
  memcpy(dst, data, 3 * sizeof(float));
}

Edge::Edge(const Point3f* p1, const Point3f* p2)
  : p1_(p1), p2_(p2), middle_point_(0) {}

const Point3f* Edge::MiddlePoint(Point3f* middle_point) {
  if (middle_point) {
    middle_point->data[0] = (p1_->data[0] + p2_->data[0]) / 2;
    middle_point->data[1] = (p1_->data[1] + p2_->data[1]) / 2;
    middle_point->data[2] = (p1_->data[2] + p2_->data[2]) / 2;
    middle_point_ = middle_point;
  }
  return middle_point_;
}

bool Edge::CompareTo(unsigned p1_id, unsigned p2_id) const {
  return p1_->id == p1_id && p2_->id == p2_id ||
         p1_->id == p2_id && p2_->id == p1_id;
}

Triangle::Triangle(const Point3f* v1, const Point3f* v2, const Point3f* v3,
                   Edge* e1, Edge* e2, Edge* e3) {
  points_[0] = v1;
  points_[1] = v2;
  points_[2] = v3;
  edges_[0] = e1;
  edges_[1] = e2;
  edges_[2] = e3;

  float xs[3], ys[3], zs[3];
  for (unsigned i = 0; i < 3; ++i) {
    xs[i] = points_[i]->data[0];
    ys[i] = points_[i]->data[1];
    zs[i] = points_[i]->data[2];
  }
}

void Triangle::GetIndices(uint16_t* dst) const {
  for (unsigned i = 0; i < 3; ++i) {
    dst[i] = points_[i]->id;
  }
}

void Triangle::GetMiddlePointsIndices(uint16_t* dst) const {
  for (unsigned i = 0; i < 3; ++i) {
    dst[i] = edges_[i]->MiddlePoint()->id;
  }
}
