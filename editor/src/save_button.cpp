// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/save_button.h"

#include <iostream>
#include <string>

SaveButton::SaveButton(const Icosphere* icosphere,
                       ConsoleViewListener* console_view_listener)
  : Button("Save"), icosphere_(icosphere), waiting_path_(false),
    console_view_listener_(console_view_listener) {}

void SaveButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    waiting_path_ = true;
    // Reset current command, if exists.
    console_view_listener_->ProcessCommand();
  }
}

void SaveButton::DoEvents() {
  if (waiting_path_) {
    std::string command = console_view_listener_->ProcessCommand();
    if (command != "") {
      icosphere_->Save(command);
      std::cout << "Model saved as " << command << std::endl;
      waiting_path_ = false;
    }
  }
}
