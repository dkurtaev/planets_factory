// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef MODEL_INCLUDE_ICOSPHERE_H_
#define MODEL_INCLUDE_ICOSPHERE_H_

#include <stdint.h>

#include <vector>
#include <string>

#include "include/structures.h"

class Icosphere {
 public:
  explicit Icosphere(float radius, const std::string& src_file = "");

  ~Icosphere();

  void Build(const std::string& src_file);

  // Drawing textured mesh.
  void Draw() const;

  // Drawing icosphere grid.
  void DrawGrid() const;

  std::vector<Point3f*>* GetVertices();

  std::vector<Triangle*>* GetTriangles();

  // Writing binary file with data of mesh. Editor's internal format.
  void Save(const std::string& file_path) const;


 private:
  void Clear();

  // Setup texture coordinates for icosahedon.
  void SetTexCoords();

  // Used for building icosphere from icosahedron.
  void AddTriangle(uint16_t v1, uint16_t v2, uint16_t v3,
                   std::vector<Edge*>* edges);

  // Used for building icosphere from icosahedron.
  void SplitTriangles(std::vector<Edge*>* edges);

  std::vector<Point3f*> vertices_;
  std::vector<Triangle*> triangles_;
  float* vertices_array_;
  uint16_t* indices_array_;
  uint16_t* tex_coord_array_;
  // Using for drawing icosphere grid. One color for internal vertices, other
  // color for vertices on initial edges.
  uint8_t* colors_array_;
  float radius_;
  uint8_t n_splits_;
};

#endif  // MODEL_INCLUDE_ICOSPHERE_H_
