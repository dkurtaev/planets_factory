// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_METRICS_VIEW_H_
#define EDITOR_INCLUDE_METRICS_VIEW_H_

#include <stdint.h>

#include <sys/time.h>
#include <queue>

#include "include/glview.h"
#include "include/glview_listener.h"

class MetricsView;
class ParentShapeListener : public GLViewListener {
 public:
  ParentShapeListener(MetricsView* child_);

  virtual void Reshape(int parent_width, int parent_height);

 private:
  MetricsView* child_view_;
};

class MetricsView : public GLView {
 public:
  MetricsView(GLView* parent);

  virtual void Display();

  void ParentIsReshaped(int parent_width, int parent_height);

 private:
  static const uint8_t kFontColor[];
  static const unsigned kViewHeight = 25;
  static const unsigned kViewWidth = 100;

  void TimeCheck();

  // Time in milliseconds when collects frames times for computing mean fps.
  static const unsigned kPeriod = 5000;
  std::queue<timeval> frames_times_;
  ParentShapeListener parent_shape_listener_;
};

#endif  // EDITOR_INCLUDE_METRICS_VIEW_H_
