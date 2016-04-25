#ifndef INCLUDE_SPHERICAL_CS_H_
#define INCLUDE_SPHERICAL_CS_H_

class SphericalCS {
 public:
  // Axises ids.
  static const unsigned ORDINATE = 0;
  static const unsigned NORMAL = 4;
  static const unsigned ABSCISSA = 8;

  // Model matrix mapping.
  static const unsigned ORDINATE_X = 0;
  static const unsigned ORDINATE_Y = 1;
  static const unsigned ORDINATE_Z = 2;
  static const unsigned NORMAL_X = 4;
  static const unsigned NORMAL_Y = 5;
  static const unsigned NORMAL_Z = 6;
  static const unsigned ABSCISSA_X = 8;
  static const unsigned ABSCISSA_Y = 9;
  static const unsigned ABSCISSA_Z = 10;
  static const unsigned POSITION_X = 12;
  static const unsigned POSITION_Y = 13;
  static const unsigned POSITION_Z = 14;

  SphericalCS(float radius = 0, float azimuth = 0, float zenith = 0,
              float own_rotation = 0, SphericalCS* center = 0);

  ~SphericalCS();

  void Rotate(unsigned axis_id, float angle);

  void GetModelMatrix(float* matrix);

 private:
  bool GlobalMatrixNeedsToUpdate();

  SphericalCS* center_;
  float radius_;
  float* local_model_matrix_;
  float* global_model_matrix_;
  bool update_global_matrix_;
};

#endif  // INCLUDE_SPHERICAL_CS_H_
