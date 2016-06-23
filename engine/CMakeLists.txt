cmake_minimum_required(VERSION 2.8)

project(engine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(sources
  src/glview.cpp
  src/button.cpp
  src/layout.cpp
)

set(headers	
  include/layout.h
  include/glview_listener.h
  include/glview.h
  include/button.h
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
)
