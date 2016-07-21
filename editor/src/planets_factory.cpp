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
#include "include/vertices_mover.h"
#include "include/save_button.h"
#include "include/load_button.h"
#include "include/console_view.h"
#include "include/console_view_listener.h"
#include "include/backtrace.h"

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
  Backtrace backtrace;

  std::vector<Triangle*>* init_triangles = icosphere.GetInitTriangles();
  std::vector<Triangle*>* triangles = icosphere.GetTriangles();
  std::vector<Point3f*>* vertices = icosphere.GetVertices();

  cv::Mat texture = cv::imread("./texture.png");
  if (!texture.data) {
    texture = cv::Mat(512, 1024, CV_8UC3);
    texture.setTo(255);
  }
  Switcher texture_colorizer_enable_switcher("Color");
  TextureColorizer texture_colorizer(&texture, init_triangles,
                                     &change_color_button,
                                     &brush_size_button,
                                     &texture_colorizer_enable_switcher,
                                     &backtrace);

  Switcher move_up_mover_switcher("Terrain up");
  Switcher move_down_mover_switcher("Terrain down");
  VerticesMover vertices_mover(vertices, &move_up_mover_switcher,
                               &move_down_mover_switcher,
                               &backtrace);
  std::vector<Switcher*> radio_group;
  move_up_mover_switcher.AddToRadioGroup(&radio_group);
  move_down_mover_switcher.AddToRadioGroup(&radio_group);
  texture_colorizer_enable_switcher.AddToRadioGroup(&radio_group);

  bool draw_grid = false;
  bool draw_mesh = true;
  bool use_sun_shading = false;
  Switcher draw_grid_switcher("Grid", &draw_grid);
  Switcher draw_mesh_switcher("Mesh", &draw_mesh);
  Switcher sun_shading_switcher("Sun shader", &use_sun_shading);

  PlanetView planet_view(&icosphere, &camera_cs, &texture, &draw_grid,
                         &draw_mesh, &use_sun_shading, &texture_colorizer,
                         &vertices_mover);
  planet_view.AddListener(&camera_mover);
  planet_view.AddListener(&texture_colorizer);
  planet_view.AddListener(&vertices_mover);
  planet_view.AsRootView();

  icosphere.BindTexCoordsVBO();

  ConsoleView console_view(&planet_view);

  SaveButton save_button(&icosphere, &console_view);
  LoadButton load_button(&icosphere, &console_view, &backtrace);

  ConsoleViewListener console_view_listener(&console_view, &save_button,
                                            &draw_grid_switcher,
                                            &texture_colorizer_enable_switcher,
                                            &backtrace);
  planet_view.AddListener(&console_view_listener);

  std::vector<Button*> buttons;
  buttons.push_back(&texture_colorizer_enable_switcher);
  buttons.push_back(&change_color_button);
  buttons.push_back(&brush_size_button);
  buttons.push_back(&move_up_mover_switcher);
  buttons.push_back(&move_down_mover_switcher);
  buttons.push_back(&sun_shading_switcher);
  buttons.push_back(&draw_grid_switcher);
  buttons.push_back(&draw_mesh_switcher);
  buttons.push_back(&save_button);
  buttons.push_back(&load_button);
  ActionsView actions_view(buttons, &planet_view);
  actions_view.AddListener(&console_view_listener);

  MetricsView metrics_view(&planet_view, *vertices, *triangles, texture);
  metrics_view.AddListener(&console_view_listener);

  glutMainLoop();
  return 0;
}
