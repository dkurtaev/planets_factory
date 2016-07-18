// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/load_button.h"

#include <iostream>
#include <string>

LoadButton::LoadButton(Icosphere* icosphere, ConsoleView* console_view,
                       Backtrace* backtrace)
  : Button("Load"), icosphere_(icosphere), console_view_(console_view),
    backtrace_(backtrace) {}

void LoadButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    console_view_->Write("Enter command \"load\" [<path>]");
    // Reset current command, if exists.
    std::string command;
    console_view_->ProcessCommand(&command);
  }
}

void LoadButton::DoEvents() {
  static const std::string kCommandPrefix = "load ";

  std::string command;
  console_view_->ProcessCommand(&command);
  if (command != "") {
    int idx = command.find(kCommandPrefix);
    if (idx != -1) {
      command = command.substr(idx + kCommandPrefix.size());
      icosphere_->Build(command);
      backtrace_->Clear();
      console_view_->Write("Model loaded from " + command);
    } else {
      console_view_->ReturnCommand(command);
    }
  }
}
