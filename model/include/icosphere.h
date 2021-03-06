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
  Icosphere(float radius, unsigned n_splits,
            const std::string& tex_coords_file);

  ~Icosphere();

  void Build(const std::string& src_file, float radius = 1.0f,
             unsigned n_splits = 0);

  // Drawing textured mesh.
  void Draw();

  // Drawing icosphere grid.
  void DrawGrid() const;

  std::vector<Point3f*>* GetVertices();

  std::vector<Triangle*>* GetTriangles();

  std::vector<Triangle*>* GetInitTriangles();

  // Writing binary file with data of mesh. Editor's internal format.
  void Save(const std::string& file_path) const;

  uint8_t GetNumSplits() const;

  void Update();

 private:
  void Clear();

  // Setup texture coordinates for icosahedon.
  void SetTexCoords();

  // Used for building icosphere from icosahedron.
  void AddTriangle(uint16_t v1, uint16_t v2, uint16_t v3,
                   std::vector<Edge*>* edges);

  // Used for building icosphere from icosahedron.
  void SplitTriangles(std::vector<Edge*>* edges);

  void UpdateVBOs();

  std::vector<Point3f*> vertices_;
  // Actual triangles.
  std::vector<Triangle*> triangles_;
  // Initial icosahedron triangles.
  std::vector<Triangle*> init_triangles_;
  // All triangles.
  std::vector<Triangle*> all_triangles_;
  uint16_t* indices_array_;
  uint16_t* tex_coord_array_;
  unsigned n_splits_;
  unsigned coordinates_vbo_;
  unsigned norms_vbo_;
  unsigned normals_vbo_;
  unsigned tex_coords_vbo_;
  bool need_to_update_vbo_;
  std::string tex_coords_file_;
};

#endif  // MODEL_INCLUDE_ICOSPHERE_H_
