// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef ENGINE_INCLUDE_GLVIEW_H_
#define ENGINE_INCLUDE_GLVIEW_H_

#include <vector>
#include <string>

#include "include/glview_listener.h"

class GLView {
 public:
  GLView(int display_width, int display_height, std::string window_header = "",
         GLView* parent = 0, int sub_x = 0, int sub_y = 0);

  ~GLView();

  void AddListener(GLViewListener* listener);

  // First opened window initializing OpenGL's context. After it we cannot use
  // gl* functions like glGenBuffers for VBO. This function creates and destroy
  // fictive window that initializing OpenGL's context.
  static void InitGLContext();

  int GetHeight() const;

  int GetWidth() const;

  void Hide();

  // After closing root window terminates all inherited views.
  void AsRootView();

 protected:
  virtual void Display() = 0;

  int display_width_;
  int display_height_;
  int window_handle_;

 private:
  void InitWindow(std::string window_header, GLView* parent, int sub_x,
                  int sub_y);

  static void Reshape(int width, int height);

  static void IdleDisplay();

  static void SpecialKeyPressed(int key, int x, int y);

  static void SpecialKeyReleased(int key, int x, int y);

  static void MouseFunc(int button, int state, int x, int y);

  static void MouseMove(int x, int y);

  static void PassiveMouseMove(int x, int y);

  static void CloseFunc();

  static void EntryFunc(int state);

  static void KeyPressed(uint8_t key, int x, int y);

  static GLView* GetActiveGLView();

  static std::vector<GLView*> inherited_views_;
  std::vector<GLViewListener*> listeners_;
  bool root_;
};

#endif  // ENGINE_INCLUDE_GLVIEW_H_
