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
#include "include/structures.h"

class MetricsView : public GLView {
 public:
  MetricsView(GLView* parent, const std::vector<Point3f*>& vertices,
              const std::vector<Triangle*>& triangles, const cv::Mat& texture);

  virtual void Display();

 private:
  static const uint8_t kFontColor[];

  void TimeCheck();

  void DrawTable(std::stringstream* ss);

  // Time in milliseconds when collects frames times for computing mean fps.
  static const unsigned kPeriod = 5000;
  std::queue<timeval> frames_times_;
  unsigned n_triangles_;
  unsigned n_vertices_;
  cv::Point texture_size_;
  GLView* parent_;
};

#endif  // EDITOR_INCLUDE_METRICS_VIEW_H_
