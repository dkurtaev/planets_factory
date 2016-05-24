#include "include/structures.h"

#include <stdlib.h>
#include <string.h>

Point3f::Point3f(float x, float y, float z, unsigned id)
  : id(id) {
  data[0] = x;
  data[1] = y;
  data[2] = z;
}

void Point3f::GetCoordinates(float* dst) {
  memcpy(dst, data, 3 * sizeof(float));
}

Edge::Edge(const Point3f* p1, const Point3f* p2)
  : p1_(p1), p2_(p2), middle_(0) {}

Edge::~Edge() {
  delete middle_;
}

Point3f* Edge::Split(unsigned& available_point_id) {
  if (!middle_) {
    middle_ = new Point3f((p1_->data[0] + p2_->data[0]) / 2,
                          (p1_->data[1] + p2_->data[1]) / 2,
                          (p1_->data[2] + p2_->data[2]) / 2,
                          available_point_id);
    ++available_point_id;
  }
  return middle_;
}

bool Edge::CompareTo(unsigned p1_id, unsigned p2_id) const {
  return p1_->id == p1_id && p2_->id == p2_id ||
         p1_->id == p2_id && p2_->id == p1_id;
}

const Point3f* Edge::GetPoint(unsigned idx) const {
  return (idx == 0 ? p1_ : (idx == 1 ? p2_ : 0));
}

Triangle::Triangle(unsigned v1, unsigned v2, unsigned v3,
                   const Edge* e1, const Edge* e2, const Edge* e3) {
  indices_[0] = v1;
  indices_[1] = v2;
  indices_[2] = v3;
  edges_[0] = e1;
  edges_[1] = e2;
  edges_[2] = e3;

  for (unsigned i = 0; i < 3; ++i) {
    points_[i] = edges_[i]->GetPoint(0);
  }

  float xs[3], ys[3], zs[3];
  for (unsigned i = 0; i < 3; ++i) {
    xs[i] = points_[i]->data[0];
    ys[i] = points_[i]->data[1];
    zs[i] = points_[i]->data[2];
  }

  normal_.data[0] = ys[0] * (zs[2] - zs[3]) + ys[1] * (zs[3] - zs[1]) +
                    ys[2] * (zs[1] - zs[2]);
  normal_.data[1] = xs[0] * (zs[3] - zs[2]) + xs[1] * (zs[1] - zs[3]) +
                    xs[2] * (zs[2] - zs[1]);
  normal_.data[2] = xs[0] * (ys[2] - ys[3]) + xs[1] * (ys[3] - ys[1]) +
                    xs[2] * (ys[1] - ys[2]);
}

void Triangle::GetIndices(unsigned short* dst) {
  memcpy(dst, indices_, sizeof(unsigned short) * 3);
}

void Triangle::GetNormal(float* dst) {
  normal_.GetCoordinates(dst);
}