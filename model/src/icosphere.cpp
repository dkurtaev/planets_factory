// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/icosphere.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <glog/logging.h>

#include "include/shaders_factory.h"

Icosphere::Icosphere(float radius, const std::string& src_file)
  : radius_(radius), vertices_array_(0), indices_array_(0), tex_coord_array_(0),
    colors_array_(0) {
  Build(src_file);
}

Icosphere::~Icosphere() {
  Clear();
}

void Icosphere::Clear() {
  unsigned size = vertices_.size();
  for (unsigned i = 0; i < size; ++i) {
    delete vertices_[i];
  }
  vertices_.clear();

  size = triangles_.size();
  for (unsigned i = 0; i < size; ++i) {
    delete triangles_[i];
  }
  triangles_.clear();

  delete[] vertices_array_;
  delete[] indices_array_;
  delete[] colors_array_;
  delete[] tex_coord_array_;
}

void Icosphere::Build(const std::string& src_file) {
  // Characteristics of icosahedron.
  static const unsigned kInitNumTriangles = 20;
  static const unsigned kInitNumVertices = 12;
  static const unsigned kInitNumEdges = 30;
  // Number of triangles splitting procedure calls.
  static const unsigned kNumSplits = 3;
  // Colors of grid nodes.
  static const uint8_t kInitEdgesColor[] = { 204, 0, 0 };
  static const uint8_t kSubEdgesColor[] = { 0, 204, 0 };

  Clear();
  if (src_file != "") {
    std::ifstream file(src_file.c_str(), std::ifstream::binary);
    CHECK(file.is_open());
    file.read(reinterpret_cast<char*>(&n_splits_), sizeof(n_splits_));
    file.close();
  } else {
    n_splits_ = kNumSplits;
  }
  // Characteristics of final icosphere.
  // After each split:
  // new_n_trianlges = 4 * prev_n_triangles
  // new_n_vertices = prev_n_vertices + 1.5 * (prev_n_triangles)
  const unsigned kNumTriangles = kInitNumTriangles * pow(4, n_splits_);
  const unsigned kNumVertices =
      kInitNumVertices + 0.5f * (kNumTriangles - kInitNumTriangles);
  const unsigned kNumEdges = (3 * kNumTriangles) / 2;

  vertices_array_ = new float[3 * kNumVertices];
  colors_array_ = new uint8_t[3 * kNumVertices];
  indices_array_ = new uint16_t[3 * kNumTriangles];
  tex_coord_array_ = new uint16_t[6 * kNumTriangles];

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
  const float h = sqrt(radius_ * radius_ / (2.5 + 0.5 * sqrt(5)));
  const float w = 0.5 * (1 + sqrt(5)) * h;

  // Vertices.
  vertices_.resize(kInitNumVertices);
  vertices_.reserve(kNumVertices);
  float xs[] = {w, w, -w, -w, 0, 0, 0, 0, -h, h, h, -h};
  float ys[] = {-h, h, h, -h, w, w, -w, -w, 0, 0, 0, 0};
  float zs[] = {0, 0, 0, 0, -h, h, h, -h, w, w, -w, -w};
  for (unsigned i = 0; i < kInitNumVertices; ++i) {
    vertices_[i] = new Point3f(i, xs[i], ys[i], zs[i],
                               vertices_array_ + i * 3,  // Offsets to
                               colors_array_ + i * 3);   // shared memory.
  }

  std::vector<Edge*> edges;
  edges.reserve(kNumEdges);
  triangles_.reserve(kNumTriangles);

  uint8_t triangles_ids[][3] = {
      {5, 9, 1}, {5, 1, 4}, {5, 4, 2}, {5, 2, 8}, {5, 8, 9}, {0, 1, 9},
      {0, 9, 6}, {0, 6, 7}, {0, 7, 10}, {0, 10, 1}, {11, 2, 4}, {11, 4, 10},
      {11, 10, 7}, {11, 7, 3}, {11, 3, 2}, {4, 1, 10}, {6, 9, 8}, {6, 8, 3},
      {3, 7, 6}, {3, 8, 2}};

  for (unsigned i = 0; i < kInitNumTriangles; ++i) {
    AddTriangle(triangles_ids[i][0], triangles_ids[i][1], triangles_ids[i][2],
                &edges);
  }

  std::vector<Edge*> init_edges(kInitNumEdges);
  for (unsigned i = 0; i < kInitNumEdges; ++i) {
    init_edges[i] = new Edge(*edges[i]);
  }

  SetTexCoords();

  for (unsigned i = 0; i < n_splits_; ++i) {
    SplitTriangles(&edges);
  }

  CHECK(triangles_.size() == kNumTriangles);
  CHECK(edges.size() == kNumEdges);
  CHECK(vertices_.size() == kNumVertices);

  for (unsigned i = 0; i < kNumTriangles; ++i) {
    triangles_[i]->GetIndices(indices_array_ + i * 3);
    triangles_[i]->GetTexCoords(tex_coord_array_ + i * 6);
  }
  Point3f* p1;
  Point3f* p2;
  for (unsigned i = 0; i < kNumEdges; ++i) {
    edges[i]->GetPoints(&p1, &p2);
    p1->AddNeighbor(p2);
    p2->AddNeighbor(p1);
  }

  Point3f* vertex;
  bool vertex_on_init_edge;
  for (unsigned i = 0; i < kNumVertices; ++i) {
    vertex = vertices_[i];
    vertex_on_init_edge = false;
    for (unsigned j = 0; j < kInitNumEdges; ++j) {
      if (init_edges[j]->IsInsideEdgeCone(*vertex)) {
        vertex_on_init_edge = true;
        break;
      }
    }
    vertex->SetColor(vertex_on_init_edge ? kInitEdgesColor : kSubEdgesColor);
  }
  for (unsigned i = 0; i < kInitNumEdges; ++i) {
    delete init_edges[i];
  }
  for (unsigned i = 0; i < kNumEdges; ++i) {
    delete edges[i];
  }
  if (src_file != "") {
    float* norms = new float[kNumVertices];
    std::ifstream file(src_file.c_str(), std::ifstream::binary);
    CHECK(file.is_open());
    file.seekg(sizeof(n_splits_));  // Skip.
    file.read(reinterpret_cast<char*>(norms), sizeof(float) * kNumVertices);
    file.close();
    for (unsigned i = 0; i < kNumVertices; ++i) {
      vertices_[i]->Normalize(norms[i]);
    }
    delete[] norms;
  }
}

void Icosphere::AddTriangle(uint16_t v1, uint16_t v2, uint16_t v3,
                            std::vector<Edge*>* edges) {
  uint16_t pairs[3][2];
  pairs[0][0] = v1; pairs[0][1] = v2;
  pairs[1][0] = v2; pairs[1][1] = v3;
  pairs[2][0] = v3; pairs[2][1] = v1;

  Edge* traingle_edges[3];
  Edge* edge = 0;
  int n_edges = edges->size();
  for (int i = 0; i < 3; ++i) {
    // Check if edge exists.
    traingle_edges[i] = 0;
    for (int j = 0; j < n_edges; ++j) {
      edge = edges->operator[](j);
      if (edge->CompareTo(pairs[i][0], pairs[i][1])) {
        traingle_edges[i] = edge;
        break;
      }
    }
    if (!traingle_edges[i]) {
      traingle_edges[i] = new Edge(vertices_[pairs[i][0]],
                                   vertices_[pairs[i][1]]);
      edges->push_back(traingle_edges[i]);
      ++n_edges;
    }
  }
  Triangle* new_triangle = new Triangle(vertices_[v1], vertices_[v2],
                                        vertices_[v3], traingle_edges[0],
                                        traingle_edges[1], traingle_edges[2]);
  triangles_.push_back(new_triangle);
}

void Icosphere::Draw() const {
  // Mesh.
  const unsigned n_tris = triangles_.size();
  float* vertices = new float[9 * n_tris];
  int8_t* normals = new int8_t[9 * n_tris];

  unsigned offset;
  float* vertices_indent = vertices;
  int8_t* normals_indent = normals;

  const uint8_t sizeof_float_x3 = sizeof(float) * 3;
  const uint8_t sizeof_uint8_t_x3 = sizeof(uint8_t) * 3;
  for (unsigned i = 0; i < n_tris; ++i) {
    triangles_[i]->GetNormal(normals_indent);
    memcpy(normals_indent + 3, normals_indent, sizeof_uint8_t_x3);
    memcpy(normals_indent + 6, normals_indent, sizeof_uint8_t_x3);
    normals_indent += 9;
    for (uint8_t j = 0; j < 3; ++j) {
      offset = indices_array_[i * 3 + j] * 3;
      memcpy(vertices_indent, vertices_array_ + offset, sizeof_float_x3);
      vertices_indent += 3;
    }
  }

  unsigned vbo[4];
  glGenBuffers(4, vbo);

  // Coordinates VBO.
  CHECK_NE(vbo[0], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * n_tris,
               vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(COORDS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(COORDS_ATTRIB);

  // Normals VBO.
  CHECK_NE(vbo[2], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int8_t) * 9 * n_tris,
               normals, GL_STATIC_DRAW);
  glVertexAttribPointer(NORMALS_ATTRIB, 3, GL_BYTE, true, 0, 0);
  glEnableVertexAttribArray(NORMALS_ATTRIB);

  // Texture coordinates VBO.
  CHECK_NE(vbo[3], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uint16_t) * 6 * triangles_.size(),
               tex_coord_array_, GL_STATIC_DRAW);
  glVertexAttribPointer(TEX_ATTRIB, 2, GL_UNSIGNED_SHORT, true, 0, 0);
  glEnableVertexAttribArray(TEX_ATTRIB);

  glDrawArrays(GL_TRIANGLES, 0, 3 * triangles_.size());

  glDisableVertexAttribArray(TEX_ATTRIB);
  glDisableVertexAttribArray(NORMALS_ATTRIB);
  glDisableVertexAttribArray(COORDS_ATTRIB);
  glDeleteBuffers(4, vbo);
  delete[] vertices;
  delete[] normals;
}

void Icosphere::SplitTriangles(std::vector<Edge*>* edges) {
  const unsigned n_triangles = triangles_.size();
  const unsigned n_vertices = vertices_.size();
  const unsigned n_edges = edges->size();

  // Split each edge in halfs.
  Point3f* middle_point;
  for (unsigned i = 0; i < n_edges; ++i) {
    const unsigned id = n_vertices + i;
    middle_point = new Point3f(id, 0, 0, 0,
                               vertices_array_ + id * 3,  // Offsets to
                               colors_array_ + id * 3);   // shared data.
    edges->operator[](i)->MiddlePoint(middle_point);
    middle_point->Normalize(radius_);
    vertices_.push_back(middle_point);
  }

  // Collect data of old triangles.
  Triangle* triangle;
  uint16_t vertices_ids[n_triangles][3];
  uint16_t middle_verts_ids[n_triangles][3];
  uint16_t tex_coords[n_triangles][6];
  uint16_t middle_verts_tex_coords[n_triangles][6];
  for (unsigned i = 0; i < n_triangles; ++i) {
    triangle = triangles_[i];
    triangle->GetIndices(vertices_ids[i]);
    triangle->GetMiddlePointsIndices(middle_verts_ids[i]);
    triangle->GetTexCoords(tex_coords[i]);
    triangle->GetMiddlePointsTexCoords(middle_verts_tex_coords[i]);
    delete triangle;
  }
  triangles_.clear();

  for (unsigned i = 0; i < n_edges; ++i) {
    delete edges->operator[](i);
  }
  edges->clear();

  // Create new triangles.
  // Split each triangle. ti - i-th old triangle's vertex,
  //                      mj - middle point of j-th edge.
  // t0_____m0_____t1
  // \      /\      /
  //  \    /  \    /
  //   \  /    \  /
  // m2 \/______\/ m1
  //     \      /
  //      \    /
  //       \  /
  //        \/ t2
  static const int orders[][3] = {{0, 3, 5}, {3, 1, 4}, {5, 4, 2}, {3, 4, 5}};
  uint16_t ids[6];  // Of t0, t1, t2, m0, m1, m2.
  uint16_t combined_tex_coords[6];
  for (unsigned i = 0; i < n_triangles; ++i) {
    memcpy(ids, vertices_ids[i], sizeof(uint16_t) * 3);  // t0, t1, t2.
    memcpy(ids + 3, middle_verts_ids[i], sizeof(uint16_t) * 3);  // m0, m1, m2.

    for (uint8_t j = 0; j < 4; ++j) {
      AddTriangle(ids[orders[j][0]], ids[orders[j][1]], ids[orders[j][2]],
                  edges);
      triangle = triangles_.back();

      for (uint8_t k = 0; k < 6; k += 2) {
        uint8_t idx = orders[j][k / 2];
        if (idx < 3) {
          idx *= 2;
          combined_tex_coords[k] = tex_coords[i][idx];
          combined_tex_coords[k + 1] = tex_coords[i][idx + 1];
        } else {
          idx = (idx - 3) * 2;
          combined_tex_coords[k] = middle_verts_tex_coords[i][idx];
          combined_tex_coords[k + 1] = middle_verts_tex_coords[i][idx + 1];
        }
      }
      triangle->SetTexCoords(combined_tex_coords);
    }
  }
}

std::vector<Point3f*>* Icosphere::GetVertices() {
  return &vertices_;
}

std::vector<Triangle*>* Icosphere::GetTriangles() {
  return &triangles_;
}

void Icosphere::SetTexCoords() {
  CHECK_EQ(triangles_.size(), 20);

  std::ifstream file("../res/tex_coords/coords_1.txt");
  uint16_t texture_coordinates[6];
  for (uint8_t i = 0; i < 20; ++i) {
    for (uint8_t j = 0; j < 6; ++j) {
      file >> texture_coordinates[j];
    }
    triangles_[i]->SetTexCoords(texture_coordinates);
  }
  file.close();
}

void Icosphere::DrawGrid() const {
  const unsigned n_tris = triangles_.size();

  unsigned vbo[3];
  glGenBuffers(3, vbo);

  // Coordinates VBO.
  CHECK_NE(vbo[0], 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertices_.size(),
               vertices_array_, GL_STATIC_DRAW);
  glVertexAttribPointer(COORDS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(COORDS_ATTRIB);

  // Colors VBO.
  CHECK_NE(vbo[1], 1);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uint8_t) * 3 * vertices_.size(),
               colors_array_, GL_STATIC_DRAW);
  glVertexAttribPointer(COLORS_ATTRIB, 3, GL_UNSIGNED_BYTE, true, 0, 0);
  glEnableVertexAttribArray(COLORS_ATTRIB);

  // Indices VBO.
  CHECK_NE(vbo[2], 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3 * n_tris,
               indices_array_, GL_STATIC_DRAW);

  glPolygonMode(GL_FRONT, GL_LINE);
  glDrawElements(GL_TRIANGLES, 3 * n_tris, GL_UNSIGNED_SHORT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDisableVertexAttribArray(COLORS_ATTRIB);
  glDisableVertexAttribArray(COORDS_ATTRIB);
  glDeleteBuffers(3, vbo);
}

void Icosphere::Save(const std::string& file_path) const {
  // Store number of splits and norms of vertices
  // (distances to icosphere center).
  std::ofstream file(file_path.c_str(), std::ofstream::binary);
  CHECK(file.is_open());
  file.write(reinterpret_cast<const char*>(&n_splits_), sizeof(n_splits_));

  const unsigned n_vertices = vertices_.size();
  float* norms = new float[n_vertices];
  for (unsigned i = 0; i < n_vertices; ++i) {
    norms[i] = vertices_[i]->GetNorm();
  }
  file.write(reinterpret_cast<char*>(norms), sizeof(float) * n_vertices);
  delete[] norms;
  file.close();
}
