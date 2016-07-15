#include "include/metrics_view.h"

#include <sstream>

#include <GL/freeglut.h>

const uint8_t MetricsView::kFontColor[] = {0, 204, 0};

MetricsView::MetricsView(GLView* parent)
  : GLView(kViewWidth, kViewHeight, "", parent,
           parent->GetWidth() - kViewWidth, kViewHeight),
    parent_shape_listener_(this) {
  if (parent) {
    parent->AddListener(&parent_shape_listener_);    
  }
}

void MetricsView::Display() {
  TimeCheck();

  // Draw info.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, display_width_, display_height_, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glLoadIdentity();

    std::stringstream ss;
    unsigned fps = static_cast<int>(frames_times_.size() * (1000.0f / kPeriod));
    ss << "fps: " << fps;

    const uint8_t* text = reinterpret_cast<const uint8_t*>(ss.str().c_str());
    const int bmp_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
    glColor3ubv(kFontColor);
    glRasterPos2i(0, bmp_height);
    glutBitmapString(GLUT_BITMAP_9_BY_15, text);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  // glDisable(GL_BLEND);
  glutSwapBuffers();
}

void MetricsView::TimeCheck() {
  // Add frame time.
  timeval current_time;
  gettimeofday(&current_time, 0);
  frames_times_.push(current_time);

  // Remove old times.
  do {
    timeval last_time = frames_times_.front();
    unsigned ms = (current_time.tv_sec - last_time.tv_sec) * 1e+3 +
                  (current_time.tv_usec - last_time.tv_usec) * 1e-3;
    if (ms > kPeriod) {
      frames_times_.pop();
    } else {
      break;
    }
  } while (!frames_times_.empty());
}

void MetricsView::ParentIsReshaped(int parent_width, int parent_height) {
  const int parent_window = glutGetWindow();
  glutSetWindow(window_handle_);
  glutPositionWindow(parent_width - display_width_, display_height_);
  glutSetWindow(parent_window);
}

ParentShapeListener::ParentShapeListener(MetricsView* child_view)
  : child_view_(child_view) {}

void ParentShapeListener::Reshape(int parent_width, int parent_height) {
  child_view_->ParentIsReshaped(parent_width, parent_height);
}
