#include "include/icosphere.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

#include <GL/freeglut.h>

Icosphere::Icosphere(float edge_length) {
  static const unsigned kNumTriangles = 20;
  static const unsigned kNumVertices = 12;

  float base_plane_height = edge_length / 2;
  float base_plane_width = (1 + sqrt(5)) * edge_length / 4;
  radius_ = sqrt(pow(base_plane_height, 2) + pow(base_plane_width, 2));

  // Vertices.
  vertices_.resize(kNumVertices);
  vertices_[0] = new Point3f(base_plane_width, -base_plane_height, 0, 0);
  vertices_[1] = new Point3f(base_plane_width, base_plane_height, 0, 1);
  vertices_[2] = new Point3f(-base_plane_width, base_plane_height, 0, 2);
  vertices_[3] = new Point3f(-base_plane_width, -base_plane_height, 0, 3);
  vertices_[4] = new Point3f(0, base_plane_width, -base_plane_height, 4);
  vertices_[5] = new Point3f(0, base_plane_width, base_plane_height, 5);
  vertices_[6] = new Point3f(0, -base_plane_width, base_plane_height, 6);
  vertices_[7] = new Point3f(0, -base_plane_width, -base_plane_height, 7);
  vertices_[8] = new Point3f(-base_plane_height, 0, base_plane_width, 8);
  vertices_[9] = new Point3f(base_plane_height, 0, base_plane_width, 9);
  vertices_[10] = new Point3f(base_plane_height, 0, -base_plane_width, 10);
  vertices_[11] = new Point3f(-base_plane_height, 0, -base_plane_width, 11);

  edges_.reserve(30);
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

  vertices_array_ = new float[3 * kNumVertices];
  normals_array_ = new float[3 * kNumTriangles];
  indices_array_ = new unsigned short[3 * kNumTriangles];

  for (unsigned i = 0; i < kNumVertices; ++i) {
    vertices_[i]->GetCoordinates(vertices_array_ + i * 3);
  }
  for (unsigned i = 0; i < kNumTriangles; ++i) {
    triangles_[i]->GetNormal(normals_array_ + i * 3);
    triangles_[i]->GetIndices(indices_array_ + i * 3);
  }

  // SplitTriangles();
  // SplitTriangles();
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

void Icosphere::Draw() {
  glColor3f(0, 0.8, 0);

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

  unsigned new_n_vertices = n_vertices + 3 * n_triangles / 2;
  unsigned new_n_triangles = n_triangles * 4;

  float* new_vertices = new float[3 * new_n_vertices];
  float* new_normals = new float[3 * new_n_triangles];
  unsigned short* new_indices = new unsigned short[3 * new_n_triangles];
  memcpy(new_vertices, vertices_array_, sizeof(float) * 3 * n_vertices);

  delete[] vertices_array_;
  delete[] normals_array_;
  delete[] indices_array_;
  vertices_array_ = new_vertices;
  normals_array_ = new_normals;
  indices_array_ = new_indices;

  // Split each edge in halfs.
  Point3f* middle_point;
  for (unsigned i = 0; i < n_edges; ++i) {
    middle_point = new Point3f(0, 0, 0, n_vertices + i);
    edges_[i]->MiddlePoint(middle_point);
    middle_point->Normalize(radius_);
    middle_point->GetCoordinates(vertices_array_ + (n_vertices + i) * 3);
    vertices_.push_back(middle_point);
  }

  // Create new triangles.
  Triangle* triangle;
  unsigned short triangle_vertices[3];
  unsigned short middle_points[3];
  for (unsigned i = 0; i < n_triangles; ++i) {
    triangle = triangles_.front();
    triangles_.erase(triangles_.begin());
    triangle->GetIndices(triangle_vertices);
    triangle->GetMiddlePointsIndices(middle_points);
    delete triangle;

    AddTriangle(triangle_vertices[0], middle_points[0], middle_points[2]);
    AddTriangle(middle_points[0], triangle_vertices[1], middle_points[1]);
    AddTriangle(middle_points[2], middle_points[1], triangle_vertices[2]);
    AddTriangle(middle_points[0], middle_points[1], middle_points[2]);
  }

  for (unsigned i = 0; i < new_n_triangles; ++i) {
    triangles_[i]->GetNormal(normals_array_ + i * 3);
    triangles_[i]->GetIndices(indices_array_ + i * 3);
  }
}
