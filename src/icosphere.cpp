#include "include/icosphere.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

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

  AddTriangle(5, 9, 1);
  AddTriangle(5, 1, 4);
  AddTriangle(5, 4, 2);
  AddTriangle(5, 2, 8);
  AddTriangle(5, 8, 9);

  AddTriangle(0, 1, 9);
  AddTriangle(0, 9, 6);
  AddTriangle(0, 6, 7);
  AddTriangle(0, 7, 10);
  AddTriangle(0, 10, 1);

  AddTriangle(11, 2, 4);
  AddTriangle(11, 4, 10);
  AddTriangle(11, 10, 7);
  AddTriangle(11, 7, 3);
  AddTriangle(11, 3, 2);

  AddTriangle(4, 1, 10);
  AddTriangle(6, 9, 8);
  AddTriangle(6, 8, 3);
  AddTriangle(3, 7, 6);
  AddTriangle(3, 8, 2);

  for (unsigned i = 0; i < kNumSplits; ++i) {
    SplitTriangles();
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
