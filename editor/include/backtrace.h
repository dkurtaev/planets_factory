// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_BACKTRACE_H_
#define EDITOR_INCLUDE_BACKTRACE_H_

#include <vector>

// Interface for actions that can be undone.
class Action {
 public:
  virtual void Undo() = 0;
};

// Undoing actions. After undoing deletes actions.
class Backtrace {
 public:
  ~Backtrace();

  void AddAction(Action* action);

  void Undo();

  void Clear();

 private:
  static const unsigned kStackDepth = 10;

  std::vector<Action*> actions_;
};

#endif  // EDITOR_INCLUDE_BACKTRACE_H_
