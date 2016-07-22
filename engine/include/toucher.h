// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef ENGINE_INCLUDE_TOUCHER_H_
#define ENGINE_INCLUDE_TOUCHER_H_

#include <queue>

#include "include/glview_listener.h"
#include "include/backtrace.h"

class Toucher : public GLViewListener {
 public:
  explicit Toucher(Backtrace* backtrace);

  virtual void MouseFunc(int button, int state, int x, int y);

  virtual void MouseMove(int x, int y);

  virtual void DoEvents();

  virtual void PassiveMouseMove(int x, int y) {}

  // Different touchers needs to different depth buffers states. This method
  // calls in cooresponding moments for solving touch requests.
  virtual void SolveTouchRequests();

 protected:
  enum Callback { MOUSE_FUNC, MOUSE_MOVE, DO_EVENTS };

  virtual void ProcessTouch(float x, float y, float z) = 0;

  // Initialize new action for writing.
  virtual void InitAction() = 0;

  // When mouse button released, write action to backtrace.
  virtual void FlushAction(Backtrace* backtrace) = 0;

  void ProcessTouch(int x, int y);

  double world_x_;
  double world_y_;
  double world_z_;
  bool infinity_touch_;
  std::queue<Callback> callback_requests_;
  std::queue<int> requests_data_;

 private:
  bool left_button_pressed_;
  Backtrace* backtrace_;
};

#endif  // ENGINE_INCLUDE_TOUCHER_H_
