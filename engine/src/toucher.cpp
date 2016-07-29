// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/toucher.h"

#include <GL/freeglut.h>

Toucher::Toucher(Backtrace* backtrace)
  : left_button_pressed_(false), backtrace_(backtrace), infinity_touch_(true) {}

void Toucher::MouseFunc(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    callback_requests_.push(MOUSE_FUNC);
    requests_data_.push(state);
    requests_data_.push(x);
    requests_data_.push(y);
  }
}

void Toucher::MouseMove(int x, int y) {
  callback_requests_.push(MOUSE_MOVE);
  requests_data_.push(x);
  requests_data_.push(y);
}

void Toucher::DoEvents() {
  callback_requests_.push(DO_EVENTS);
}

void Toucher::ProcessTouch(int x, int y) {
  int view[4];
  double model[16];
  double proj[16];
  float z = 1.0f;
  glGetIntegerv(GL_VIEWPORT, view);
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);

  y = view[3] - y;
  glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  // If not infinity.
  if (z != 1.0f) {
    infinity_touch_ = false;
    gluUnProject(x, y, z, model, proj, view, &world_x_, &world_y_, &world_z_);
    ProcessTouch(world_x_, world_y_, world_z_);
  } else {
    infinity_touch_ = true;
  }
}

void Toucher::SolveTouchRequests() {
  while (!callback_requests_.empty()) {
    Callback callback = callback_requests_.front();
    callback_requests_.pop();
    switch (callback) {
      case MOUSE_FUNC: {
        int state = requests_data_.front();
        requests_data_.pop();
        int x = requests_data_.front();
        requests_data_.pop();
        int y = requests_data_.front();
        requests_data_.pop();

        left_button_pressed_ = !static_cast<bool>(state);
        if (left_button_pressed_) {
          InitAction();
          ProcessTouch(x, y);
        } else {
          FlushAction(backtrace_);
        }
        break;
      }

      case MOUSE_MOVE: {
        int x = requests_data_.front();
        requests_data_.pop();
        int y = requests_data_.front();
        requests_data_.pop();
        if (left_button_pressed_) {
          ProcessTouch(x, y);
        }
        break;
      }

      case DO_EVENTS: {
        if (left_button_pressed_ && !infinity_touch_) {
          ProcessTouch(world_x_, world_y_, world_z_);
        }
        break;
      }
      default: break;
    }
  }
}
