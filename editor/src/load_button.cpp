// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/load_button.h"

#include <iostream>
#include <string>

#include <QFileDialog>
#include <QObject>
#include <QString>

LoadButton::LoadButton(Icosphere* icosphere, Backtrace* backtrace)
  : Button("Load"), icosphere_(icosphere), backtrace_(backtrace) {}

void LoadButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    QString title = QObject::tr("Load model");
    QString ext = QObject::tr("All files (*)");
    QString q_file_path =
        QFileDialog::getOpenFileName(0, title, "", ext, 0,
                                     QFileDialog::DontUseNativeDialog);
    std::string file_path = q_file_path.toStdString();
    if (file_path != "") {
      icosphere_->Build(file_path);
      backtrace_->Clear();
      std::cout << "Model loaded from " << file_path << std::endl;
    }
  }
}
