#ifndef INCLUDE_GLVIEW_LISTENER_H_
#define INCLUDE_GLVIEW_LISTENER_H_

#ifndef GLUT_WHEEL_UP
  #define GLUT_WHEEL_UP 3
#endif

#ifndef GLUT_WHEEL_DOWN
  #define GLUT_WHEEL_DOWN 4
#endif

class GLViewListener {
 public:
  virtual void MouseFunc(int button, int state, int x, int y) {};

  virtual void MouseMove(int x, int y) {};

  virtual void SpecialKeyPressed(int key, int x, int y) {};

  virtual void SpecialKeyReleased(int key, int x, int y) {};

  virtual void DoEvents() {};

  void Reshape(int display_width, int display_height) {
    display_width_ = display_width;
    display_height_ = display_height;
  }

 protected:
  float display_width_;
  float display_height_;
};

#endif  // INCLUDE_GLVIEW_LISTENER_H_
