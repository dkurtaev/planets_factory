// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include <vector>

#include "include/planet_view.h"
#include "include/actions_view.h"
#include "include/button.h"
#include "include/listener_enabler.h"
#include "include/change_color_button.h"
#include "include/glview.h"
#include "include/texture_colorizer.h"
#include "include/camera_mover.h"
#include "include/switcher.h"

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

  std::vector<Triangle*> triangles;
  icosphere.GetTriangles(&triangles);

  cv::Mat texture = cv::imread("./texture.png");
  CHECK(texture.data);
  TextureColorizer texture_colorizer(&texture, &triangles,
                                     &change_color_button);

  bool draw_grid = false;
  bool draw_mesh = true;
  Switcher draw_grid_switcher("Grid", &draw_grid);
  Switcher draw_mesh_switcher("Mesh", &draw_mesh);

  PlanetView planet_view(&icosphere, &camera_cs, &texture, &draw_grid,
                         &draw_mesh);
  planet_view.AddListener(&camera_mover);
  planet_view.AddListener(&texture_colorizer);

  std::vector<Button*> buttons;
  buttons.push_back(new ListenerEnabler("Camera", &camera_mover));
  buttons.push_back(new ListenerEnabler("Color", &texture_colorizer));
  buttons.push_back(&change_color_button);
  buttons.push_back(&draw_grid_switcher);
  buttons.push_back(&draw_mesh_switcher);
  ActionsView actions_view(buttons, &planet_view);

  glutMainLoop();
  return 0;
}
