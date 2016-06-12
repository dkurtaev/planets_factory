#include "include/layout.h"

#include <GL/freeglut.h>

#define FOREACH_LISTENER_IN_ROI \
  const unsigned n_listeners = listeners_.size(); \
  const float float_x = static_cast<float>(x) / display_width_; \
  const float float_y = static_cast<float>(y) / display_height_; \
  for (unsigned i = 0; i < n_listeners; ++i) \
    if (listeners_rois_[i].IsIncludes(float_x, float_y)) \
      listeners_[i]->

void Layout::AddListener(GLViewListener* listener, const Roi& roi) {
  listeners_.push_back(listener);
  listeners_rois_.push_back(roi);
}

void Layout::MouseFunc(int button, int state, int x, int y) {
  FOREACH_LISTENER_IN_ROI MouseFunc(button, state, x, y);
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
  for (unsigned i = 0; i < n_listeners; ++i) {
    const Roi* roi = &listeners_rois_[i];
    if (!roi->IsIncludes(float_x, float_y)) {
      if (roi->IsIncludes(last_x, last_y)) {
        listeners_[i]->EntryFunc(GLUT_LEFT);
      }
    } else {
      if (!roi->IsIncludes(last_x, last_y)) {
        listeners_[i]->EntryFunc(GLUT_ENTERED);        
      } else {
        if (passive) {
          listeners_[i]->PassiveMouseMove(x, y);          
        } else {
          listeners_[i]->MouseMove(x, y);
        }
      }
    }
  }
}

void Layout::SpecialKeyPressed(int key, int x, int y) {
  FOREACH_LISTENER_IN_ROI SpecialKeyPressed(key, x, y);
}

void Layout::SpecialKeyReleased(int key, int x, int y) {
  FOREACH_LISTENER_IN_ROI SpecialKeyReleased(key, x, y);
}

void Layout::DoEvents() {
  const unsigned n_listeners = listeners_.size();
  for (unsigned i = 0; i < n_listeners; ++i) {
    listeners_[i]->DoEvents();
  }
}

void Layout::EntryFunc(int state) {
  if (state == GLUT_LEFT) {
    unsigned x = last_mouse_x_;
    unsigned y = last_mouse_y_;
    FOREACH_LISTENER_IN_ROI EntryFunc(GLUT_LEFT);    
  }
}

void Layout::Reshape(int display_width, int display_height) {
  const unsigned n_listeners = listeners_.size();  
  for (unsigned i = 0; i < n_listeners; ++i) {
    const Roi* roi = &listeners_rois_[i];
    listeners_[i]->Reshape(roi->GetWidth(display_width),
                           roi->GetHeight(display_height));
  }
  display_width_ = display_width;
  display_height_ = display_height;
}
