// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_PLANET_VIEW_H_
#define EDITOR_INCLUDE_PLANET_VIEW_H_

#include <vector>

#include <opencv2/opencv.hpp>

#include "include/glview.h"
#include "include/spherical_cs.h"
#include "include/camera.h"
#include "include/icosphere.h"
#include "include/toucher.h"
#include "include/texture_colorizer.h"
#include "include/vertices_mover.h"
#include "include/grass_field.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

class HighlightingToucher : public Toucher {
 public:
  HighlightingToucher() : Toucher(0) {}

  virtual void MouseFunc(int button, int state, int x, int y) {}

  virtual void DoEvents() {}

  virtual void MouseMove(int x, int y);

  virtual void PassiveMouseMove(int x, int y);

  virtual void SolveTouchRequests();

  bool HasTouchPoint();

  void GetTouchPoint(float* dst);

 private:
  virtual void ProcessTouch(float x, float y, float z) {}

  virtual void InitAction() {}

  virtual void FlushAction(Backtrace* backtrace) {}
};

class PlanetView : public GLView {
 public:
  PlanetView(Icosphere* icosphere, SphericalCS* camera_cs,
             const cv::Mat* texture, bool* draw_grid, bool* draw_mesh,
             bool* sun_shading, TextureColorizer* texture_colorizer,
             VerticesMover* vertices_mover, GrassField* grass_field,
             bool* draw_grass,
             const std::vector<std::string>& mesh_vert_shader_srcs,
             const std::vector<std::string>& mesh_frag_shader_srcs,
             const std::vector<std::string>& grid_vert_shader_srcs,
             const std::vector<std::string>& grid_frag_shader_srcs);

  virtual void Display();

  // Icosphere touchers reads depth buffer with icosphere only.
  void AddIcosphereToucher(Toucher* toucher);

 private:
  void InitGL();

  void SetTexture();

  Camera camera_;
  Icosphere* icosphere_;
  unsigned texture_id_;
  const cv::Mat* texture_;
  unsigned planet_shader_program_;
  unsigned grid_shader_program_;
  bool* draw_grid_;
  bool* draw_mesh_;
  bool* sun_shading_;
  bool* draw_grass_;
  HighlightingToucher highlighting_toucher_;
  TextureColorizer* texture_colorizer_;
  VerticesMover* vertices_mover_;
  GrassField* grass_field_;
  std::vector<Toucher*> icosphere_touchers_;
};

#endif  // EDITOR_INCLUDE_PLANET_VIEW_H_
