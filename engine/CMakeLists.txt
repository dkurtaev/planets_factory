cmake_minimum_required(VERSION 2.8)

project(engine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(sources
  src/backtrace.cpp
  src/button.cpp
  src/glview.cpp
  src/layout.cpp
  src/toucher.cpp
)

set(headers
  include/backtrace.h
  include/button.h
  include/glview.h
  include/glview_listener.h
  include/layout.h
  include/toucher.h
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

