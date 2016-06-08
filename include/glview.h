#ifndef INCLUDE_GLVIEW_H_
#define INCLUDE_GLVIEW_H_

#include <vector>

#include "include/glview_listener.h"

class GLView {
 public:
  GLView(int display_width, int display_height);

  void AddListener(GLViewListener* listener);

 protected:
  virtual void Display() {};

  int display_width_;
  int display_height_;

 private:
  void InitWindow();

  void InitGL();

  static void Reshape(int width, int height);

  static void IdleDisplay();

  static void SpecialKeyPressed(int key, int x, int y);

  static void SpecialKeyReleased(int key, int x, int y);

  static void MouseFunc(int button, int state, int x, int y);

  static void MouseMove(int x, int y);

  static GLView* GetActiveGLView();

  static std::vector<GLView*> inherited_views_;
  int window_handle_;
  std::vector<GLViewListener*> listeners_;
};

#endif  // INCLUDE_GLVIEW_H_
