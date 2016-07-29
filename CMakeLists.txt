cmake_minimum_required(VERSION 2.8)

project(planets_factory)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${OpenGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  "./engine"
  "./model"
  "./editor"
  "./3rdparty/yaml-cpp/include"
)

add_subdirectory("./3rdparty/glog")
add_subdirectory("./3rdparty/yaml-cpp")
add_subdirectory("./engine")
add_subdirectory("./model")
add_subdirectory("./editor")
