// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/save_button.h"

#include <iostream>
#include <string>

SaveButton::SaveButton(const Icosphere* icosphere)
  : Button("Save"), icosphere_(icosphere) {}

void SaveButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    std::string file_path;
    std::cout << "Save file path: " << std::flush;
    std::cin >> file_path;
    icosphere_->Save(file_path);
    std::cout << "Model saved as " << file_path << std::endl;
  }
}
