#include <string>

#include <gtest/gtest.h>

#include "include/glview.h"
#include "include/shaders_factory.h"

static const char* kShadersRoot = "../res/shaders/";

// Add new shaders pair (vertex shader + fragment shader) for testing.
static const std::string shaders[1][2] = {
  {"test_shader.vertex", "test_shader.fragment"}
};
// Increment this value after adding new shaders pair.
static const unsigned kNumShaders = 1;

TEST(ShadersFactory, attribs_location) {
  GLView::InitGLContext();
  for (unsigned i = 0; i < kNumShaders; ++i) {
    std::string name = shaders[i][0].substr(0, shaders[i][0].find_last_of('.'));
    unsigned program = ShadersFactory::GetProgramFromFile(
                          (kShadersRoot + shaders[i][0]).c_str(),
                          (kShadersRoot + shaders[i][1]).c_str());
    ASSERT_EQ(glGetAttribLocation(program, "a_position"), 0)
        << "At \"" << name << "\"";
    ASSERT_EQ(glGetAttribLocation(program, "a_color"), 1)
        << "At \"" << name << "\"";
    ASSERT_EQ(glGetAttribLocation(program, "a_normal"), 2)
        << "At \"" << name << "\"";
    ASSERT_NE(glGetUniformLocation(program, "u_modelview_matrix"), -1)
        << "At \"" << name << "\"";
    ASSERT_NE(glGetUniformLocation(program, "u_projection_matrix"), -1)
        << "At \"" << name << "\"";
  }
}
