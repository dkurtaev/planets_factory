cmake_minimum_required(VERSION 2.8)

project(model)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(sources
  src/camera.cpp
  src/icosphere.cpp
  src/shaders_factory.cpp
  src/spherical_cs.cpp
  src/structures.cpp
)

set(headers
  include/camera.h
  include/icosphere.h
  include/shaders_factory.h
  include/spherical_cs.h
  include/structures.h
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
  ${OpenGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} STATIC ${sources} ${headers})
target_link_libraries(${PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  glog
)

