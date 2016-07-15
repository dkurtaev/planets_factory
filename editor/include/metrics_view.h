// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_METRICS_VIEW_H_
#define EDITOR_INCLUDE_METRICS_VIEW_H_

#include <stdint.h>
#include <sys/time.h>

#include <queue>
#include <vector>
#include <sstream>

#include <opencv2/opencv.hpp>

#include "include/glview.h"
#include "include/glview_listener.h"
#include "include/structures.h"

class MetricsView;
class ParentShapeListener : public GLViewListener {
 public:
  explicit ParentShapeListener(MetricsView* child_);

  virtual void Reshape(int parent_width, int parent_height);

 private:
  MetricsView* child_view_;
};

class MetricsView : public GLView {
 public:
  MetricsView(GLView* parent, const std::vector<Point3f*>& vertices,
              const std::vector<Triangle*>& triangles, const cv::Mat& texture);

  virtual void Display();

  void ParentIsReshaped(int parent_width, int parent_height);

 private:
  static const uint8_t kFontColor[];

  void TimeCheck();

  void DrawTable(std::stringstream* ss);

  // Time in milliseconds when collects frames times for computing mean fps.
  static const unsigned kPeriod = 5000;
  std::queue<timeval> frames_times_;
  ParentShapeListener parent_shape_listener_;
  unsigned n_triangles_;
  unsigned n_vertices_;
  cv::Point texture_size_;
  int parent_width_;
  int parent_height_;
};

#endif  // EDITOR_INCLUDE_METRICS_VIEW_H_
