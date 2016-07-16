// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/metrics_view.h"

#include <iomanip>
#include <string>
#include <vector>

#include <GL/freeglut.h>

const uint8_t MetricsView::kFontColor[] = {0, 204, 0};

MetricsView::MetricsView(GLView* parent, const std::vector<Point3f*>& vertices,
                         const std::vector<Triangle*>& triangles,
                         const cv::Mat& texture)
  : GLView(1, 1, "", parent), parent_(parent), n_triangles_(triangles.size()),
    n_vertices_(vertices.size()), texture_size_(texture.size()) {}

void MetricsView::Display() {
  TimeCheck();

  // Draw info.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    ss << "fps|" << fps << '\n'
       << "number of triangles|" << n_triangles_ << '\n'
       << "number of vertices|" << n_vertices_ << '\n'
       << "texture size|" << texture_size_.x << 'x' << texture_size_.y << '\n';
    DrawTable(&ss);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}

void MetricsView::DrawTable(std::stringstream* ss) {
  // Raw data looks like:
  // first_row_1th_col|1st_row_2nd_col\n2nd_row_1st_col|last_cell
  // Expected:
  // first_row_1th_col | 1st_row_2nd_col
  // 2nd_row_1st_col   | last_cell
  // We need to split data by rows and align columns by biggest cell in
  // each column.
  std::vector<std::string> lines;
  std::vector<uint8_t> cols_lengths;
  std::string str;
  while (std::getline(*ss, str, '\n')) {
    lines.push_back(str);
    std::stringstream line_ss(str);
    for (uint8_t col = 0; std::getline(line_ss, str, '|'); ++col) {
      if (col < cols_lengths.size()) {
        if (cols_lengths[col] < str.length()) {
          cols_lengths[col] = str.length();
        }
      } else {
        cols_lengths.push_back(str.length());
      }
    }
  }
  // Draw it.
  const uint8_t n_lines = lines.size();
  int bmp_height = 0;
  int bmp_length;
  for (uint8_t i = 0; i < n_lines; ++i) {
    std::stringstream dst_ss;
    std::stringstream src_ss(lines[i]);
    for (uint8_t col = 0; std::getline(src_ss, str, '|'); ++col) {
      if (col != 0) {
        dst_ss << " | ";
      }
      dst_ss << std::left << std::setfill(' ')
             << std::setw(cols_lengths[col]) << str;
    }
    str = dst_ss.str();
    const uint8_t* text = reinterpret_cast<const uint8_t*>(str.c_str());
    bmp_length = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
    bmp_height += glutBitmapHeight(GLUT_BITMAP_9_BY_15);
    glColor3ubv(kFontColor);
    glRasterPos2i(0, bmp_height);
    glutBitmapString(GLUT_BITMAP_9_BY_15, text);
  }
  glutReshapeWindow(bmp_length, bmp_height);
  if (parent_ != 0) {
    glutPositionWindow(parent_->GetWidth() - display_width_, 0);
  }
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
