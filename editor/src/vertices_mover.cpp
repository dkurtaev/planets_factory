// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/vertices_mover.h"

#include <math.h>

#include <map>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include <glog/logging.h>

VerticesMover::VerticesMover(Icosphere* icosphere,
                             Switcher* is_move_up_swither,
                             Switcher* is_move_down_swither,
                             Backtrace* backtrace)
  : VerticesToucher(icosphere->GetVertices(), backtrace), is_move_up_(false),
    is_move_down_(false), icosphere_(icosphere) {
  is_move_up_swither->SetFlag(&is_move_up_);
  is_move_down_swither->SetFlag(&is_move_down_);
}

void VerticesMover::DoAction(Point3f* vertex) {
  static const float kIncrement = 1e-3f;

  CHECK(!is_move_up_ || !is_move_down_);

  float norm = vertex->GetNorm();
  vertex->Normalize(norm + (is_move_up_ ? kIncrement : -kIncrement));
  if (action_data_.find(vertex) == action_data_.end()) {
    action_data_[vertex] = norm;
  }

  std::vector<Point3f*> area_level(1, vertex);
  std::vector<Point3f*> neighborhood;
  std::vector<Point3f*> unique_neighbors;
  std::set<Point3f*> unique_vertices;
  std::vector<Point3f*>::iterator it;

  unique_vertices.insert(vertex);
  for (unsigned i = 1; i <= kAreaRadius; ++i) {
    for (it = area_level.begin(); it != area_level.end(); ++it) {
      (*it)->GetNeighborhood(&neighborhood);
      const unsigned n_neighbors = neighborhood.size();
      for (unsigned j = 0; j < n_neighbors; ++j) {
        vertex = neighborhood[j];
        if (unique_vertices.insert(vertex).second) {  // If new vertex.
          unique_neighbors.push_back(vertex);
        }
      }
    }

    area_level.resize(unique_neighbors.size());
    std::copy(unique_neighbors.begin(), unique_neighbors.end(),
              area_level.begin());
    unique_neighbors.clear();

    for (it = area_level.begin(); it != area_level.end(); ++it) {
      vertex = *it;
      norm = vertex->GetNorm();
      float ratio = 1.0f - static_cast<float>(i) / (1.0f + kAreaRadius);
      float power = kIncrement * ratio;
      vertex->Normalize(norm + (is_move_up_ ? power : -power));
      if (action_data_.find(vertex) == action_data_.end()) {
        action_data_[vertex] = norm;
      }
    }
  }
  icosphere_->Update();
}

bool VerticesMover::IsEnabled() {
  return is_move_up_ || is_move_down_;
}

void VerticesMover::InitAction() {
  action_data_.clear();
}

void VerticesMover::FlushAction(Backtrace* backtrace) {
  backtrace->AddAction(new VerticesMoverAction(action_data_));
}

float VerticesMover::GetHighlightingAngle(uint8_t n_splits) const {
  return (kAreaRadius + 1) * M_PI / (3.0f * (1 << n_splits));
}

VerticesMoverAction::VerticesMoverAction(const std::map<Point3f*, float>& data)
  : data_(data) {}

void VerticesMoverAction::Undo() {
  std::map<Point3f*, float>::iterator it;
  for (it = data_.begin(); it != data_.end(); ++it) {
    (*it).first->Normalize((*it).second);
  }
}
