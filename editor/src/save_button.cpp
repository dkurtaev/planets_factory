// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/save_button.h"

#include <string>

SaveButton::SaveButton(const Icosphere* icosphere, ConsoleView* console_view)
  : Button("Save"), icosphere_(icosphere), console_view_(console_view),
    last_path_("") {}

void SaveButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    Save();
  }
}

void SaveButton::DoEvents() {
  static const std::string kCommandPrefix = "save";

  std::string command;
  console_view_->ProcessCommand(&command);
  if (command != "") {
    int idx = command.find(kCommandPrefix);  // Check prefix.
    if (idx != -1) {
      if (command != kCommandPrefix) {
        command = command.substr(idx + (kCommandPrefix + ' ').size());
        last_path_ = command;
      }
      Save();
    } else {
      console_view_->ReturnCommand(command);
    }
  }
}

void SaveButton::Save() {
  if (last_path_ != "") {
    icosphere_->Save(last_path_);
    console_view_->Write("Model saved as " + last_path_);
  } else {
    console_view_->Write("Enter command \"save\" [<path>]");
    // Reset current command, if exists.
    std::string command;
    console_view_->ProcessCommand(&command);
  }
}
