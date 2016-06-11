#ifndef INCLUDE_GLVIEW_H_
#define INCLUDE_GLVIEW_H_

#include <vector>
#include <string>

#include "include/glview_listener.h"

struct Roi {
  float top;
  float left;
  float height;
  float width;

  Roi(float top = 0.0f, float left = 0.0f,
      float height = 1.0f, float width = 1.0f)
    : top(top), left(left), height(height), width(width) {}

  bool IsIncludes(float x, float y);
};

class GLView {
 public:
  GLView(int display_width, int display_height, std::string window_header);

  void AddListener(GLViewListener* listener, const Roi& roi = Roi());

 protected:
  virtual void Display() = 0;

  int display_width_;
  int display_height_;
  std::vector<Roi> listeners_rois_;

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

  // Using for detecting mouse entry/leave for ROIs.
  unsigned last_mouse_x_;
  unsigned last_mouse_y_;
  // For ROIs first mouse movement as entry event.
  bool mouse_moved_;
};

#endif  // INCLUDE_GLVIEW_H_
