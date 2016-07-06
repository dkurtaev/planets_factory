// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef INCLUDE_PLANET_VIEW_H_
#define INCLUDE_PLANET_VIEW_H_

#include <opencv2/opencv.hpp>

#include "include/glview.h"
#include "include/spherical_cs.h"
#include "include/camera.h"
#include "include/icosphere.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

class PlanetView : public GLView {
 public:
  PlanetView(const Icosphere* icosphere, SphericalCS* camera_cs);

  virtual void Display();

 private:
  void InitGL();

  void LoadTexture();

  Camera camera_;
  const Icosphere* icosphere_;
  unsigned texture_id_;
  cv::Mat texture_;
  unsigned planet_shader_program_;
  unsigned grid_shader_program_;
};

#endif  // INCLUDE_PLANET_VIEW_H_
