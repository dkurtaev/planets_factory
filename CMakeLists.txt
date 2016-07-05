cmake_minimum_required(VERSION 2.8)

project(planets_factory)

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/")
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${OpenGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  "./engine"
)

file(GLOB_RECURSE sources "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE headers "${PROJECT_INCLUDE_DIR}/*.h")

add_subdirectory("./3rdparty/glog")
add_subdirectory("./3rdparty/googletest")
add_subdirectory("./engine")
add_subdirectory("./test")

add_executable(${CMAKE_PROJECT_NAME} ${sources} ${headers})
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  engine
  glog
)
