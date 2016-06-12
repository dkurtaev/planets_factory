#ifndef INCLUDE_GLVIEW_H_
#define INCLUDE_GLVIEW_H_

#include <vector>
#include <string>

#include "include/glview_listener.h"

class GLView {
 public:
  GLView(int display_width, int display_height, std::string window_header);

  void AddListener(GLViewListener* listener);

 protected:
  virtual void Display() = 0;

  int display_width_;
  int display_height_;

 private:
  void InitWindow(std::string window_header);

  static void Reshape(int width, int height);

  static void IdleDisplay();

  static void SpecialKeyPressed(int key, int x, int y);

  static void SpecialKeyReleased(int key, int x, int y);

  static void MouseFunc(int button, int state, int x, int y);

  static void MouseMove(int x, int y);

  static void PassiveMouseMove(int x, int y);

  static void CloseFunc();

  static void EntryFunc(int state);

  static GLView* GetActiveGLView();

  static std::vector<GLView*> inherited_views_;
  int window_handle_;
  std::vector<GLViewListener*> listeners_;
};

#endif  // INCLUDE_GLVIEW_H_
