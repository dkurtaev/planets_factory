#include "include/layout.h"

#include <GL/freeglut.h>

#define FOREACH_LISTENER_IN_ROI \
  const unsigned n_listeners = listeners_.size(); \
  const float float_x = static_cast<float>(x) / display_width_; \
  const float float_y = static_cast<float>(y) / display_height_; \
  for (unsigned i = 0; i < n_listeners; ++i) \
    if (listeners_rois_[i].IsIncludes(float_x, float_y)) \
      if (listeners_[i]->IsEnabled())

void Layout::AddListener(GLViewListener* listener, const Roi& roi) {
  listeners_.push_back(listener);
  listeners_rois_.push_back(roi);
}

void Layout::MouseFunc(int button, int state, int x, int y) {
  unsigned top, left, right, bottom;
  FOREACH_LISTENER_IN_ROI {
    listeners_rois_[i].Get(display_width_, display_height_, &left, &right,
                           &top, &bottom);
    listeners_[i]->MouseFunc(button, state, x - left, y - top);
  }
}

void Layout::MouseMove(int x, int y) {
  MouseMove(false, x, y);
}

void Layout::PassiveMouseMove(int x, int y) {
  MouseMove(true, x, y);
}

void Layout::MouseMove(bool passive, int x, int y) {
  const unsigned n_listeners = listeners_.size();
  const float float_x = static_cast<float>(x) / display_width_;
  const float float_y = static_cast<float>(y) / display_height_;
  const float last_x = static_cast<float>(last_mouse_x_) / display_width_;
  const float last_y = static_cast<float>(last_mouse_y_) / display_height_;
  last_mouse_x_ = x;
  last_mouse_y_ = y;
  unsigned top, left, right, bottom;
  for (unsigned i = 0; i < n_listeners; ++i) {
    if (listeners_[i]->IsEnabled()) {
      const Roi* roi = &listeners_rois_[i];
      if (!roi->IsIncludes(float_x, float_y)) {
        if (roi->IsIncludes(last_x, last_y)) {
          listeners_[i]->EntryFunc(GLUT_LEFT);
        }
      } else {
        if (!roi->IsIncludes(last_x, last_y)) {
          listeners_[i]->EntryFunc(GLUT_ENTERED);
        } else {
          roi->Get(display_width_, display_height_, &left, &right, &top,
                   &bottom);
          if (passive) {
            listeners_[i]->PassiveMouseMove(x - left, y - top);
          } else {
            listeners_[i]->MouseMove(x - left, y - top);
          }
        }
      }
    }
  }
}

void Layout::SpecialKeyPressed(int key, int x, int y) {
  unsigned top, left, right, bottom;
  FOREACH_LISTENER_IN_ROI {
    listeners_rois_[i].Get(display_width_, display_height_, &left, &right,
                           &top, &bottom);
    listeners_[i]->SpecialKeyPressed(key, x - left, y - top);
  }
}

void Layout::SpecialKeyReleased(int key, int x, int y) {
  unsigned top, left, right, bottom;
  FOREACH_LISTENER_IN_ROI {
    listeners_rois_[i].Get(display_width_, display_height_, &left, &right,
                           &top, &bottom);
    listeners_[i]->SpecialKeyReleased(key, x - left, y - top);
  }
}

void Layout::DoEvents() {
  const unsigned n_listeners = listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    if (listeners_[i]->IsEnabled()) {
      listeners_[i]->DoEvents();
    }
  }
}

void Layout::EntryFunc(int state) {
  if (state == GLUT_LEFT) {
    unsigned x = last_mouse_x_;
    unsigned y = last_mouse_y_;
    FOREACH_LISTENER_IN_ROI {
      listeners_[i]->EntryFunc(GLUT_LEFT);
    }
  }
}

void Layout::Reshape(int display_width, int display_height) {
  const unsigned n_listeners = listeners_.size();  
  for (unsigned i = 0; i < n_listeners; ++i) {
    if (listeners_[i]->IsEnabled()) {
      const Roi* roi = &listeners_rois_[i];
      listeners_[i]->Reshape(roi->GetWidth(display_width),
                             roi->GetHeight(display_height));
    }
  }
  display_width_ = display_width;
  display_height_ = display_height;
}
