#ifndef INCLUDE_STRUCTURES_H_
#define INCLUDE_STRUCTURES_H_

struct Point3f {
  Point3f(float x = 0, float y = 0, float z = 0, unsigned short id = 0);

  void Normalize(float target_norm);

  void GetCoordinates(float* dst) const;

  unsigned short id;
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

  const Point3f* GetPoint(unsigned idx) const;

 private:
  const Point3f* p1_;
  const Point3f* p2_;
  const Point3f* middle_point_;
};

struct Triangle {
  Triangle(const Point3f* v1, const Point3f* v2, const Point3f* v3,
           Edge* e1, Edge* e2, Edge* e3);

  void GetIndices(unsigned short* dst) const;

  void GetMiddlePointsIndices(unsigned short* dst) const;

  void GetNormal(float* dst) const;

  Edge* edges_[3];
  const Point3f* points_[3];
  Point3f normal_;
};

#endif  // INCLUDE_STRUCTURES_H_
