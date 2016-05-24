#ifndef INCLUDE_STRUCTURES_H_
#define INCLUDE_STRUCTURES_H_

struct Point3f {
  Point3f(float x = 0, float y = 0, float z = 0, unsigned id = -1);

  void GetCoordinates(float* dst);

  unsigned id;
  float data[3];
};

class Edge {
 public:
  Edge(const Point3f* p1, const Point3f* p2);

  ~Edge();

  Point3f* Split(unsigned& available_point_id);

  bool CompareTo(unsigned p1_id, unsigned p2_id) const;

  const Point3f* GetPoint(unsigned idx) const;

 private:
  const Point3f* p1_;
  const Point3f* p2_;
  Point3f* middle_;
};

struct Triangle {
  Triangle(unsigned v1, unsigned v2, unsigned v3,
           const Edge* e1, const Edge* e2, const Edge* e3);

  void GetIndices(unsigned short* dst);

  void GetNormal(float* dst);

  const Edge* edges_[3];
  const Point3f* points_[3];
  unsigned short indices_[3];
  Point3f normal_;
};

#endif  // INCLUDE_STRUCTURES_H_
