#ifndef INCLUDE_STRUCTURES_H_
#define INCLUDE_STRUCTURES_H_

#include <stdint.h>

struct Point3f {
  explicit Point3f(float x = 0, float y = 0, float z = 0, uint16_t id = 0);

  void Normalize(float target_norm);

  void GetCoordinates(float* dst) const;

  uint16_t id;
  float data[3];
};

class Edge {
 public:
  Edge(const Point3f* p1, const Point3f* p2);

  // Write middle point's coordinates into input Point3f.
  // If null as input - returns last passed middle point
  // (or null if not called yet).
  const Point3f* MiddlePoint(Point3f* middle_point = 0);

  bool CompareTo(unsigned p1_id, unsigned p2_id) const;

 private:
  const Point3f* p1_;
  const Point3f* p2_;
  const Point3f* middle_point_;
};

struct Triangle {
  Triangle(const Point3f* v1, const Point3f* v2, const Point3f* v3,
           Edge* e1, Edge* e2, Edge* e3);

  void GetIndices(uint16_t* dst) const;

  void GetMiddlePointsIndices(uint16_t* dst) const;

  Edge* edges_[3];
  const Point3f* points_[3];
};

#endif  // INCLUDE_STRUCTURES_H_
