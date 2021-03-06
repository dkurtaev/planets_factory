// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef MODEL_INCLUDE_STRUCTURES_H_
#define MODEL_INCLUDE_STRUCTURES_H_

#include <stdint.h>

#include <vector>
#include <sys/time.h>

class Edge;
class Point3f {
 public:
  explicit Point3f(uint16_t id, float x = 0, float y = 0, float z = 0);

  static void MiddlePoint(const Point3f& p1, const Point3f& p2,
                          Point3f* middle_point);

  float SquaredDistanceTo(float x, float y, float z);

  void Normalize(float target_norm);

  void SetPosition(float x, float y, float z);

  void GetPosition(float* x, float* y, float* z) const;

  void GetPosition(float* dst) const;

  void GetPosition(int16_t* dst, float* norm) const;

  void SetColor(const uint8_t* src);

  void GetColor(uint8_t* dst) const;

  float GetNorm() const { return norm_; }

  uint16_t GetId() const { return id_; }

  // Neighborhood methods.
  void AddNeighbor(Point3f* point, Edge* edge);

  void ResetNeighborhood();

  // Returns 0 if not neighbor.
  Edge* GetEdgeTo(const Point3f* point) const;

  void GetNeighborhood(std::vector<Point3f*>* neighborhood);

 private:
  uint16_t id_;
  float norm_;
  std::vector<Point3f*> neighborhood_;
  std::vector<Edge*> edges_;
  std::vector<uint8_t> color_;
  std::vector<float> coordinates_;
};

class Edge {
 public:
  Edge(Point3f* p1, Point3f* p2);

  // Write middle point's coordinates into input Point3f.
  // If null as input - returns last passed middle point
  // (or null if not called yet).
  const Point3f* MiddlePoint(Point3f* middle_point = 0);

  bool CompareTo(uint16_t p1_id, uint16_t p2_id) const;

  void GetPoints(Point3f** p1, Point3f** p2);

  // Check that point inside cone based on vectors from (0, 0, 0) to p1 and p2.
  //   \   *p     /
  // p1 \        / p2
  //     *------*
  //      \    /
  //       \  /
  //        \/ 0
  bool IsInsideEdgeCone(const Point3f& p);

 private:
  Point3f* p1_;
  Point3f* p2_;
  const Point3f* middle_point_;
};

class Triangle {
 public:
  Triangle(const Point3f* v1, const Point3f* v2, const Point3f* v3,
           Edge* e1, Edge* e2, Edge* e3);

  ~Triangle();

  // Getters.
  void GetIndices(uint16_t* dst) const;

  void GetTexCoords(uint16_t* dst) const;

  void GetTexCoords(float* dst) const;

  // Triangle's normal. Not normalized.
  void GetNormal(int8_t* dst) const;

  // Triangle's normal. Normalized.
  void GetNormal(float* dst) const;

  // Writing vertices space coordinates into array of 9 floats.
  void GetCoords(float* dst) const;

  void GetMiddlePointsIndices(uint16_t* dst) const;

  void GetMiddlePointsTexCoords(uint16_t* dst) const;

  void GetSubtriangles(std::vector<Triangle*>* subtriangles) const;

  // Setters.
  void SetTexCoords(const uint16_t* src);

  void AddSubtriangle(Triangle* subtriangle);

  // Checkers.
  // Returns true if point p is inside triangle's cone.
  // Cone vertex at (0, 0, 0).
  // p1______p2
  //   \ *p /
  //    \  /
  //     \/ p3
  // If true, set barycentric coordinates bary_p* relatively triangle's points.
  bool IsInsideCone(float x, float y, float z, float* bary_p1, float* bary_p2,
                    float* bary_p3) const;

 private:
  std::vector<Edge*> edges_;
  std::vector<const Point3f*> points_;
  uint16_t* texture_coordinates_;
  std::vector<Triangle*> subtriangles_;
};

float Determinant(float* col_1, float* col_2, float* col_3);

float Determinant(float* col_1, float* col_2);

void Normalize(float* vec);

float TimeFrom(const timeval& tv);

#endif  // MODEL_INCLUDE_STRUCTURES_H_
