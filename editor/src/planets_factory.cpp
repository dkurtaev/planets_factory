// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include <vector>
#include <fstream>

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
#include "include/save_load_buttons.h"
#include "include/shortcuts_listener.h"
#include "include/backtrace.h"
#include "include/grass_field.h"
#include "include/grass_grower.h"

#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>
#include <glog/logging.h>
#include <QApplication>
#include <yaml-cpp/yaml.h>

struct CameraParameters {
  CameraParameters();

  float orbit_radius;
  float azimuth;
  float zenith;
  float own_rotation;
};

struct IcosphereParameters {
  IcosphereParameters();

  float radius;
  unsigned n_splits;
  std::string texture_coords_file;
};

struct PlanetParameters {
  cv::Mat texture;
  std::vector<std::string> mesh_vert_shader_srcs;
  std::vector<std::string> mesh_frag_shader_srcs;
  std::vector<std::string> grid_vert_shader_srcs;
  std::vector<std::string> grid_frag_shader_srcs;
};

struct GrassParameters {
  std::vector<std::string> vertex_shader_sources;
  std::vector<std::string> fragment_shader_sources;
  cv::Mat texture_color;
  cv::Mat texture_alpha;
};

void operator >> (const YAML::Node& config, CameraParameters& params);

void operator >> (const YAML::Node& config, IcosphereParameters& params);

void operator >> (const YAML::Node& config, PlanetParameters& params);

void operator >> (const YAML::Node& config, GrassParameters& params);

void operator >> (const YAML::Node& config, std::vector<std::string>& vec);

int main(int argc, char** argv) {
  GLView::InitGLContext();
  QApplication app(argc, argv);

  std::ifstream config_file("../config.yaml");
  CHECK(config_file.is_open()) << "Not found configuration file ../config.yaml";

  YAML::Parser parser(config_file);
  YAML::Node root_config_node;
  parser.GetNextDocument(root_config_node);

  CameraParameters camera_params;
  IcosphereParameters ico_params;
  PlanetParameters planet_params;
  GrassParameters grass_params;
  root_config_node["camera"] >> camera_params;
  root_config_node["icosphere"] >> ico_params;
  root_config_node["planet"] >> planet_params;
  root_config_node["grass"] >> grass_params;

  SphericalCS identity_cs;
  SphericalCS camera_cs(camera_params.orbit_radius, camera_params.azimuth,
                        camera_params.zenith, camera_params.own_rotation,
                        &identity_cs);

  CameraMover camera_mover(&camera_cs);
  Icosphere icosphere(ico_params.radius, ico_params.n_splits,
                      ico_params.texture_coords_file);
  ChangeColorButton change_color_button;
  BrushSizeButton brush_size_button;
  Backtrace backtrace;
  GrassField grass_field;

  std::vector<Triangle*>* init_triangles = icosphere.GetInitTriangles();
  std::vector<Triangle*>* triangles = icosphere.GetTriangles();
  std::vector<Point3f*>* vertices = icosphere.GetVertices();

  Switcher texture_colorizer_enable_switcher("Color");
  TextureColorizer texture_colorizer(&planet_params.texture, init_triangles,
                                     &change_color_button,
                                     &brush_size_button,
                                     &texture_colorizer_enable_switcher,
                                     &backtrace);

  Switcher move_up_mover_switcher("Terrain up");
  Switcher move_down_mover_switcher("Terrain down");
  VerticesMover vertices_mover(&icosphere, &move_up_mover_switcher,
                               &move_down_mover_switcher,
                               &backtrace, &grass_field);

  Switcher grass_grower_switcher("Grass");
  GrassGrower grass_grower(&grass_field, triangles, &backtrace,
                           &grass_grower_switcher);

  std::vector<Switcher*> radio_group;
  move_up_mover_switcher.AddToRadioGroup(&radio_group);
  move_down_mover_switcher.AddToRadioGroup(&radio_group);
  texture_colorizer_enable_switcher.AddToRadioGroup(&radio_group);
  grass_grower_switcher.AddToRadioGroup(&radio_group);

  bool draw_grid = false;
  bool draw_mesh = true;
  bool use_sun_shading = false;
  bool draw_grass = true;
  Switcher draw_grid_switcher("Grid", &draw_grid);
  Switcher draw_mesh_switcher("Mesh", &draw_mesh);
  Switcher sun_shading_switcher("Sun shading", &use_sun_shading);
  Switcher draw_grass_switcher("Draw grass", &draw_grass);

  PlanetView planet_view(&icosphere, &camera_cs, &planet_params.texture,
                         &draw_grid, &draw_mesh, &use_sun_shading,
                         &texture_colorizer, &vertices_mover, &grass_field,
                         &draw_grass, planet_params.mesh_vert_shader_srcs,
                         planet_params.mesh_frag_shader_srcs,
                         planet_params.grid_vert_shader_srcs,
                         planet_params.grid_frag_shader_srcs);
  planet_view.AddListener(&camera_mover);
  planet_view.AddIcosphereToucher(&texture_colorizer);
  planet_view.AddIcosphereToucher(&vertices_mover);
  planet_view.AddIcosphereToucher(&grass_grower);
  planet_view.AsRootView();

  grass_field.Init(grass_params.vertex_shader_sources,
                   grass_params.fragment_shader_sources,
                   grass_params.texture_color, grass_params.texture_alpha);

  SaveButton save_button(&icosphere, &texture_colorizer);
  LoadButton load_button(&icosphere, &backtrace, &texture_colorizer);

  ShortcutsListener shortcuts_listener(&save_button, &draw_grid_switcher,
                                       &texture_colorizer_enable_switcher,
                                       &backtrace);
  planet_view.AddListener(&shortcuts_listener);

  std::vector<Button*> buttons;
  buttons.push_back(&texture_colorizer_enable_switcher);
  buttons.push_back(&change_color_button);
  buttons.push_back(&brush_size_button);
  buttons.push_back(&move_up_mover_switcher);
  buttons.push_back(&move_down_mover_switcher);
  buttons.push_back(&grass_grower_switcher);
  buttons.push_back(&sun_shading_switcher);
  buttons.push_back(&draw_grid_switcher);
  buttons.push_back(&draw_mesh_switcher);
  buttons.push_back(&draw_grass_switcher);
  buttons.push_back(&save_button);
  buttons.push_back(&load_button);
  ActionsView actions_view(buttons, "../actions_view_config.yaml",
                           &planet_view);
  actions_view.AddListener(&shortcuts_listener);

  MetricsView metrics_view(&planet_view, *vertices, *triangles,
                           planet_params.texture);
  metrics_view.AddListener(&shortcuts_listener);

  glutMainLoop();
  config_file.close();
  return 0;
}

void operator >> (const YAML::Node& config, CameraParameters& params) {
  config["orbit_radius"] >> params.orbit_radius;
  config["init_azimuth"] >> params.azimuth;
  config["init_zenith"] >> params.zenith;
  config["init_own_rotation"] >> params.own_rotation;
}

void operator >> (const YAML::Node& config, IcosphereParameters& params) {
  config["radius"] >> params.radius;
  config["splits"] >> params.n_splits;
  config["texture_coords_file"] >> params.texture_coords_file;
}

void operator >> (const YAML::Node& config, PlanetParameters& params) {
  std::string path;
  config["texture"] >> path;
  params.texture = cv::imread(path);
  if (!params.texture.data) {
    int width, height;
    config["default_texture_width"] >> width;
    config["default_texture_height"] >> height;
    params.texture = cv::Mat(height, width, CV_8UC3);
    params.texture.setTo(255);
  }
  config["mesh_vertex_shader"] >> params.mesh_vert_shader_srcs;
  config["mesh_fragment_shader"] >> params.mesh_frag_shader_srcs;
  config["grid_vertex_shader"] >> params.grid_vert_shader_srcs;
  config["grid_fragment_shader"] >> params.grid_frag_shader_srcs;
}

void operator >> (const YAML::Node& config, GrassParameters& params) {
  unsigned default_texture_size;
  std::string path;
  config["default_texture_size"] >> default_texture_size;

  config["texture_color"] >> path;
  params.texture_color = cv::imread(path);
  if (!params.texture_color.data) {
    params.texture_color = cv::Mat(default_texture_size, default_texture_size,
                                   CV_8UC3);
    params.texture_color.setTo(cv::Scalar(0, 255, 0));
  }

  config["texture_alpha"] >> path;
  params.texture_alpha = cv::imread(path);
  if (!params.texture_alpha.data) {
    params.texture_alpha = cv::Mat(default_texture_size, default_texture_size,
                                   CV_8UC3);
    params.texture_alpha.setTo(255);
  }
  config["vertex_shader"] >> params.vertex_shader_sources;
  config["fragment_shader"] >> params.fragment_shader_sources;
}


void operator >> (const YAML::Node& config, std::vector<std::string>& vec) {
  vec.resize(config.size());
  for (unsigned i = 0; i < config.size(); ++i) {
    config[i]["src"] >> vec[i];
  }
}

CameraParameters::CameraParameters()
  : orbit_radius(0.0f), azimuth(0.0f), zenith(0.0f), own_rotation(0.0f) {}

IcosphereParameters::IcosphereParameters()
  : radius(1.0f), n_splits(0) {}
