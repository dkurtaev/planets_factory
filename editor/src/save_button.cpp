// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/save_button.h"

#include <string>

SaveButton::SaveButton(const Icosphere* icosphere, ConsoleView* console_view)
  : Button("Save"), icosphere_(icosphere), waiting_path_(false),
    console_view_(console_view) {}

void SaveButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    waiting_path_ = true;
    console_view_->Write("Enter command \"save\" [<path>]");
    // Reset current command, if exists.
    std::string command;
    console_view_->ProcessCommand(&command);
  }
}

void SaveButton::DoEvents() {
  static const std::string kCommandPrefix = "save ";

  if (waiting_path_) {
    std::string command;
    console_view_->ProcessCommand(&command);
    if (command != "") {
      int idx = command.find(kCommandPrefix);
      if (idx != -1) {
        command = command.substr(idx + kCommandPrefix.size());
        icosphere_->Save(command);
        console_view_->Write("Model saved as " + command);
        waiting_path_ = false;
      } else {
        console_view_->ReturnCommand(command);
      }
    }
  }
}
