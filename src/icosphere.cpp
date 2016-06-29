#include "include/icosphere.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <vector>
#include <iostream>

#include <GL/freeglut.h>

Icosphere::Icosphere(float radius) {
  // Characteristics of icosahedron.
  static const unsigned kInitNumTriangles = 20;
  static const unsigned kInitNumVertices = 12;
  static const unsigned kInitNumEdges = 30;
  // Number of triangles splitting procedure calls.
  static const unsigned kNumSplits = 3;
  // Characteristics of final icosphere.
  // After each split:
  // new_n_trianlges = 4 * prev_n_triangles
  // new_n_vertices = prev_n_vertices + 1.5 * (prev_n_triangles)
  static const unsigned kNumTriangles = kInitNumTriangles * pow(4, kNumSplits);
  static const unsigned kNumVertices =
      kInitNumVertices + 0.5f * (kNumTriangles - kInitNumTriangles);
  static const unsigned kNumEdges = (3 * kNumTriangles) / 2;

  vertices_array_ = new float[3 * kNumVertices];
  normals_array_ = new float[3 * kNumVertices];
  colors_array_ = new uint8_t[3 * kNumVertices];
  indices_array_ = new uint16_t[3 * kNumTriangles];

  // e - edges length
  // h - base plane half-height
  // w - base plane half-width
  // r - radius on icosphere
  // h = e / 2
  // w = e * (1 + sqrt(5)) / 4
  // rr = hh + ww
  // Let r known.
  // rr = hh + hh (1+sqrt(5))^2 / 4
  // h = sqrt(rr / (1 + (1+sqrt(5))^2 / 4))
  // h = sqrt(rr / (1 + (1+5+2sqrt(5)) / 4))
  // h = sqrt(rr / (1 + 1.5 + sqrt(5) / 2))
  const float h = sqrt(radius * radius / (2.5 + 0.5 * sqrt(5)));
  const float w = 0.5 * (1 + sqrt(5)) * h;
  radius_ = radius;

  // Vertices.
  vertices_.resize(kInitNumVertices);
  vertices_.reserve(kNumVertices);
  float xs[] = {w, w, -w, -w, 0, 0, 0, 0, -h, h, h, -h};
  float ys[] = {-h, h, h, -h, w, w, -w, -w, 0, 0, 0, 0};
  float zs[] = {0, 0, 0, 0, -h, h, h, -h, w, w, -w, -w};
  for (unsigned i = 0; i < kInitNumVertices; ++i) {
    vertices_[i] = new Point3f(i, xs[i], ys[i], zs[i], vertices_array_ + i * 3,
                               colors_array_ + i * 3);
  }

  edges_.reserve(kNumEdges);
  triangles_.reserve(kNumTriangles);

  uint8_t triangles_ids[][3] = {
      {5, 9, 1}, {5, 1, 4}, {5, 4, 2}, {5, 2, 8}, {5, 8, 9}, {0, 1, 9},
      {0, 9, 6}, {0, 6, 7}, {0, 7, 10}, {0, 10, 1}, {11, 2, 4}, {11, 4, 10},
      {11, 10, 7}, {11, 7, 3}, {11, 3, 2}, {4, 1, 10}, {6, 9, 8}, {6, 8, 3},
      {3, 7, 6}, {3, 8, 2}};

  for (unsigned i = 0; i < kInitNumTriangles; ++i) {
    AddTriangle(triangles_ids[i][0], triangles_ids[i][1], triangles_ids[i][2]);
  }

  for (unsigned i = 0; i < kNumSplits; ++i) {
    SplitTriangles();
  }

  if (triangles_.size() != kNumTriangles || edges_.size() != kNumEdges ||
      vertices_.size() != kNumVertices) {
    std::cout << "Icosphere building failed." << std::endl;
  }

  memset(colors_array_, kInitialColor, sizeof(uint8_t) * 3 * kNumVertices);
  memcpy(normals_array_, vertices_array_, sizeof(float) * 3 * kNumVertices);
  const unsigned dim = 3 * kNumVertices;
  for (unsigned i = 0; i < dim; ++i) {
    normals_array_[i] /= radius_;
  }
  for (unsigned i = 0; i < kNumTriangles; ++i) {
    triangles_[i]->GetIndices(indices_array_ + i * 3);
  }
  Point3f* p1;
  Point3f* p2;
  for (unsigned i = 0; i < kNumEdges; ++i) {
    edges_[i]->GetPoints(&p1, &p2);
    p1->AddNeighbor(p2);
    p2->AddNeighbor(p1);
  }
}

Icosphere::~Icosphere() {
  unsigned size = vertices_.size();
  for (unsigned i = 0; i < size; ++i) {
    delete vertices_[i];
  }

  size = edges_.size();
  for (unsigned i = 0; i < size; ++i) {
    delete edges_[i];
  }

  size = triangles_.size();
  for (unsigned i = 0; i < size; ++i) {
    delete triangles_[i];
  }

  delete[] vertices_array_;
  delete[] normals_array_;
  delete[] indices_array_;
  delete[] colors_array_;
}

void Icosphere::AddTriangle(unsigned v1, unsigned v2, unsigned v3) {
  unsigned pairs[3][2];
  pairs[0][0] = v1; pairs[0][1] = v2;
  pairs[1][0] = v2; pairs[1][1] = v3;
  pairs[2][0] = v3; pairs[2][1] = v1;

  Edge* traingle_edges[3];
  int n_edges = edges_.size();
  for (int i = 0; i < 3; ++i) {
    // Check if edge exists.
    traingle_edges[i] = 0;
    for (int j = 0; j < n_edges; ++j) {
      if (edges_[j]->CompareTo(pairs[i][0], pairs[i][1])) {
        traingle_edges[i] = edges_[j];
        break;
      }
    }
    if (!traingle_edges[i]) {
      traingle_edges[i] = new Edge(vertices_[pairs[i][0]],
                                   vertices_[pairs[i][1]]);
      edges_.push_back(traingle_edges[i]);
      ++n_edges;
    }
  }
  Triangle* new_triangle = new Triangle(vertices_[v1], vertices_[v2],
                                        vertices_[v3], traingle_edges[0],
                                        traingle_edges[1], traingle_edges[2]);
  triangles_.push_back(new_triangle);
}

void Icosphere::Draw() const {
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors_array_);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normals_array_);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices_array_);
  glDrawElements(GL_TRIANGLES, 3 * triangles_.size(), GL_UNSIGNED_SHORT,
                 indices_array_);

  glDisableClientState(GL_COLOR_ARRAY);
  glColor3f(0, 0.8, 0);
  glPolygonMode(GL_FRONT, GL_LINE);
  glDrawElements(GL_TRIANGLES, 3 * triangles_.size(), GL_UNSIGNED_SHORT,
                 indices_array_);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Icosphere::SplitTriangles() {
  const unsigned n_triangles = triangles_.size();
  const unsigned n_vertices = vertices_.size();
  const unsigned n_edges = edges_.size();

  // Split each edge in halfs.
  Point3f* middle_point;
  for (unsigned i = 0; i < n_edges; ++i) {
    const unsigned id = n_vertices + i;
    middle_point = new Point3f(id, 0, 0, 0, vertices_array_ + id * 3,
                               colors_array_ + id * 3);
    edges_[i]->MiddlePoint(middle_point);
    middle_point->Normalize(radius_);
    vertices_.push_back(middle_point);
  }

  // Create new triangles.
  Triangle* triangle;
  uint16_t triangle_verts[n_triangles][3];
  uint16_t middle_points[n_triangles][3];
  for (unsigned i = 0; i < n_triangles; ++i) {
    triangle = triangles_[i];
    triangle->GetIndices(triangle_verts[i]);
    triangle->GetMiddlePointsIndices(middle_points[i]);
    delete triangle;
  }
  triangles_.clear();

  for (unsigned i = 0; i < n_edges; ++i) {
    delete edges_[i];
  }
  edges_.clear();

  for (unsigned i = 0; i < n_triangles; ++i) {
    AddTriangle(triangle_verts[i][0], middle_points[i][0], middle_points[i][2]);
    AddTriangle(middle_points[i][0], triangle_verts[i][1], middle_points[i][1]);
    AddTriangle(middle_points[i][2], middle_points[i][1], triangle_verts[i][2]);
    AddTriangle(middle_points[i][0], middle_points[i][1], middle_points[i][2]);
  }
}

void Icosphere::GetVertices(std::vector<Point3f*>* vertices) {
  vertices->resize(vertices_.size());
  std::copy(vertices_.begin(), vertices_.end(), vertices->begin());
}

void Icosphere::SetTexCoords() {
// [01] - triangle idx.
// 01 - vertex idx.
// Above hardcoded triangles indices. We use it for map texture coordinates.
// Unwrapped icosahedron:
//     0.00  0.09  0.18  0.27  0.36  0.45  0.54  0.63  0.72  0.81  0.90  1.00
//      +------------------------------------------------------------------+
// 0.00 |*****/\**********/\**********/\**********/\**********/\***********|
//      |****/05\********/05\********/05\********/05\********/05\**********|
//      |***/    \******/    \******/    \******/    \******/    \*********|
//      |**/ [00] \****/ [01] \****/ [02] \****/ [03] \****/ [04] \********|
//      |*/        \**/        \**/        \**/        \**/        \*******|
// 0.33 |/09 ____ 01\/01 ____ 04\/04 ____ 02\/02 ____ 08\/08 ____ 09\******|
//      |\09      01/\01      04/\04      02/\02      08/\08      09/\*****|
//      |*\        /01\        /04\        /02\        /08\        /09\****|
//      |**\ [05] /    \ [15] /    \ [10] /    \ [19] /    \ [16] /    \***|
//      |***\    / [09] \    / [11] \    / [14] \    / [17] \    / [06] \**|
//      |****\00/        \10/        \11/        \03/        \06/        \*|
// 0.66 |*****\/00 ____ 10\/10 ____ 11\/11 ____ 03\/03 ____ 06\/06 ____ 00\|
//      |******\00      10/\10      11/\11      03/\03      06/\06      00/|
//      |*******\        /**\        /**\        /**\        /**\        /*|
//      |********\ [08] /****\ [12] /****\ [13] /****\ [18] /****\ [07] /**|
//      |*********\    /******\    /******\    /******\    /******\    /***|
//      |**********\07/********\07/********\07/********\07/********\07/****|
// 1.00 |***********\/**********\/**********\/**********\/**********\/*****|
//      +------------------------------------------------------------------+
  float texture_coordinates[][6] = {
    { 0.09f, 0.00f, 0.00f, 0.33f, 0.18f, 0.33f },  // [00]: {05,09,01}
    { 0.27f, 0.00f, 0.18f, 0.33f, 0.36f, 0.33f },  // [01]: {05,01,04}
    { 0.45f, 0.00f, 0.36f, 0.33f, 0.54f, 0.33f },  // [02]: {05,04,02}
    { 0.63f, 0.00f, 0.54f, 0.33f, 0.72f, 0.33f },  // [03]: {05,02,08}
    { 0.81f, 0.00f, 0.72f, 0.33f, 0.90f, 0.33f },  // [04]: {05,08,09}
    { 0.09f, 0.66f, 0.18f, 0.33f, 0.00f, 0.33f },  // [05]: {00,01,09}
    { 1.00f, 0.66f, 0.90f, 0.33f, 0.81f, 0.66f },  // [06]: {00,09,06}
    { 1.00f, 0.66f, 0.81f, 0.66f, 0.90f, 1.00f },  // [07]: {00,06,07}
    { 0.09f, 0.66f, 0.18f, 1.00f, 0.27f, 0.66f },  // [08]: {00,07,10}
    { 0.09f, 0.66f, 0.27f, 0.66f, 0.18f, 0.33f },  // [09]: {00,10,01}
    { 0.45f, 0.66f, 0.54f, 0.33f, 0.36f, 0.33f },  // [10]: {11,02,04}
    { 0.45f, 0.66f, 0.36f, 0.33f, 0.27f, 0.66f },  // [11]: {11,04,10}
    { 0.45f, 0.66f, 0.27f, 0.66f, 0.36f, 1.00f },  // [12]: {11,10,07}
    { 0.45f, 0.66f, 0.54f, 1.00f, 0.63f, 0.66f },  // [13]: {11,07,03}
    { 0.45f, 0.66f, 0.63f, 0.66f, 0.54f, 0.33f },  // [14]: {11,03,02}
    { 0.36f, 0.33f, 0.18f, 0.33f, 0.27f, 0.66f },  // [15]: {04,01,10}
    { 0.81f, 0.66f, 0.90f, 0.33f, 0.72f, 0.33f },  // [16]: {06,09,08}
    { 0.81f, 0.66f, 0.72f, 0.33f, 0.63f, 0.66f },  // [17]: {06,08,03}
    { 0.63f, 0.66f, 0.72f, 1.00f, 0.81f, 0.66f },  // [18]: {03,07,06}
    { 0.63f, 0.66f, 0.72f, 0.33f, 0.54f, 0.33f }   // [19]: {03,08,02}
  };
  if (triangles_.size() != 20) {
    std::cout << "Use testure coordinates setting before icosphere building."
              << std::endl;
  }
  for (unsigned i = 0; i < 20; ++i) {
    triangles_[i]->SetTexCoords(texture_coordinates[i]);
  }
}
