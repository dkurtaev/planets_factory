#ifndef INCLUDE_ICOSPHERE_H_
#define INCLUDE_ICOSPHERE_H_

class Icosphere {
 public:
  Icosphere(float base_plane_width, float base_plane_height);

  ~Icosphere();

  void Draw();

 private:
  void SetPoint(float x, float y, float z, float* dst);

  void SetTriangle(int triangle_idx, int vert_1, int vert_2, int vert_3);

//  void FillNormals();

  float base_plane_width_;
  float base_plane_height_;
  float* vertices_;
  float* normals_;
  unsigned short* indices_;
};

#endif  // INCLUDE_ICOSPHERE_H_
