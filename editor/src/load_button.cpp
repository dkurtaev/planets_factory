// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/load_button.h"

#include <iostream>
#include <string>

LoadButton::LoadButton(Icosphere* icosphere)
  : Button("Load"), icosphere_(icosphere) {}

void LoadButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    std::string file_path;
    std::cout << "Load file path: " << std::flush;
    std::cin >> file_path;
    icosphere_->Build(file_path);
    std::cout << "Model loaded from " << file_path << std::endl;
  }
}
