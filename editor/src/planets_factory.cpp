// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include <vector>

#include "include/planet_view.h"
#include "include/actions_view.h"
#include "include/button.h"
#include "include/change_color_button.h"
#include "include/glview.h"
#include "include/texture_colorizer.h"
#include "include/camera_mover.h"
#include "include/switcher.h"
#include "include/brush_size_button.h"
#include "include/metrics_view.h"

#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>
#include <glog/logging.h>

int main(int argc, char** argv) {
  GLView::InitGLContext();

  SphericalCS identity_cs;
  SphericalCS camera_cs(20, 0, 80, 0, &identity_cs);
  CameraMover camera_mover(&camera_cs);
  Icosphere icosphere(4);
  ChangeColorButton change_color_button;
  BrushSizeButton brush_size_button;

  std::vector<Triangle*> triangles;
  std::vector<Point3f*> vertices;
  icosphere.GetTriangles(&triangles);
  icosphere.GetVertices(&vertices);

  cv::Mat texture = cv::imread("./texture.png");
  if (!texture.data) {
    texture = cv::Mat(512, 1024, CV_8UC3);
    texture.setTo(255);
  }
  Switcher texture_colorizer_enable_switcher("Color");
  TextureColorizer texture_colorizer(&texture, &triangles,
                                     &change_color_button,
                                     &brush_size_button,
                                     &texture_colorizer_enable_switcher);

  bool draw_grid = false;
  bool draw_mesh = true;
  Switcher draw_grid_switcher("Grid", &draw_grid);
  Switcher draw_mesh_switcher("Mesh", &draw_mesh);

  PlanetView planet_view(&icosphere, &camera_cs, &texture, &draw_grid,
                         &draw_mesh);
  planet_view.AddListener(&camera_mover);
  planet_view.AddListener(&texture_colorizer);
  planet_view.AsRootView();

  std::vector<Button*> buttons;
  buttons.push_back(&texture_colorizer_enable_switcher);
  buttons.push_back(&change_color_button);
  buttons.push_back(&brush_size_button);
  buttons.push_back(&draw_grid_switcher);
  buttons.push_back(&draw_mesh_switcher);
  ActionsView actions_view(buttons, &planet_view);

  MetricsView metrics_view(&planet_view, vertices, triangles, texture);

  glutMainLoop();
  return 0;
}