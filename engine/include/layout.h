// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef ENGINE_INCLUDE_LAYOUT_H_
#define ENGINE_INCLUDE_LAYOUT_H_

#include <vector>

#include "include/glview_listener.h"

// Class for split single window (GLView) to rectangles of interest (ROI)
// for different listeners (GLViewListeners).
class Roi;
class Layout : public GLViewListener {
 public:
  void AddListener(GLViewListener* listener, const Roi& roi);

  virtual void MouseFunc(int button, int state, int x, int y);

  virtual void MouseMove(int x, int y);

  virtual void PassiveMouseMove(int x, int y);

  virtual void SpecialKeyPressed(int key, int x, int y);

  virtual void SpecialKeyReleased(int key, int x, int y);

  virtual void DoEvents();

  virtual void EntryFunc(int state);

  virtual void Reshape(int display_width, int display_height);

 private:
  void MouseMove(bool passive, int x, int y);

  std::vector<GLViewListener*> listeners_;
  std::vector<Roi> listeners_rois_;

  // Using for detecting mouse entry/leave for ROIs.
  unsigned last_mouse_x_;
  unsigned last_mouse_y_;
};

class Roi {
 public:
  explicit Roi(float left = 0, float right = 1, float top = 0, float bottom = 1)
    : left_(left), right_(right), top_(top), bottom_(bottom) {}

  bool IsIncludes(float x, float y) const;

  unsigned GetWidth(unsigned display_width) const;

  unsigned GetHeight(unsigned display_height) const;

  void Get(unsigned display_width, unsigned display_height,
           unsigned* left, unsigned* right, unsigned* top,
           unsigned* bottom) const;

 private:
  float top_, left_, right_, bottom_;
};

#endif  // ENGINE_INCLUDE_LAYOUT_H_
