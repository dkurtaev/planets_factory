// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_SAVE_LOAD_BUTTONS_H_
#define EDITOR_INCLUDE_SAVE_LOAD_BUTTONS_H_

#include <string>

#include "include/button.h"
#include "include/icosphere.h"
#include "include/backtrace.h"

#include <opencv2/opencv.hpp>

class SaveButton : public Button {
 public:
  SaveButton(const Icosphere* icosphere, const cv::Mat* texture);

  virtual void MouseFunc(int button, int state, int x, int y);

  void Save();

 private:
  const Icosphere* icosphere_;
  const cv::Mat* texture_;
  std::string last_model_path_;
  std::string last_texture_path_;
};

class LoadButton : public Button {
 public:
  // Clearing backtrace when loading new model.
  LoadButton(Icosphere* icosphere, Backtrace* backtrace);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  Icosphere* icosphere_;
  Backtrace* backtrace_;
};

#endif  // EDITOR_INCLUDE_SAVE_LOAD_BUTTONS_H_
