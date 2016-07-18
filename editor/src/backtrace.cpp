// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/backtrace.h"

Backtrace::~Backtrace() {
  Clear();
}

void Backtrace::AddAction(Action* action) {
  actions_.push_back(action);
  if (actions_.size() > kStackDepth) {
    actions_.erase(actions_.begin());
  }
}

void Backtrace::Undo() {
  if (!actions_.empty()) {
    actions_.back()->Undo();
    delete actions_.back();
    actions_.pop_back();
  }
}

void Backtrace::Clear() {
  const unsigned n_actions = actions_.size();
  for (unsigned i = 0; i < n_actions; ++i) {
    delete actions_[i];
  }
  actions_.clear();
}
