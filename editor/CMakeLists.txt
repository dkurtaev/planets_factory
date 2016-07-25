cmake_minimum_required(VERSION 2.8)

set(PROJECT_INCLUDE_DIR "./include/")
set(PROJECT_SOURCE_DIR "./src/")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5Widgets)

file(GLOB_RECURSE sources "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE headers "${PROJECT_INCLUDE_DIR}/*.h")

add_executable(${CMAKE_PROJECT_NAME} ${sources} ${headers})
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  ${OpenCV_LIBRARIES}
  engine
  model
  glog
  yaml-cpp
  Qt5::Widgets
)
