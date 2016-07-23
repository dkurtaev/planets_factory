// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/save_button.h"

#include <string>
#include <iostream>

#include <QFileDialog>
#include <QObject>
#include <QString>

SaveButton::SaveButton(const Icosphere* icosphere)
  : Button("Save"), icosphere_(icosphere), last_path_("") {}

void SaveButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    QString title = QObject::tr("Save model");
    QString ext = QObject::tr("All files (*)");
    QString q_file_path =
        QFileDialog::getSaveFileName(0, title, "", ext, 0,
                                     QFileDialog::DontUseNativeDialog);
    std::string file_path = q_file_path.toStdString();
    if (file_path != "") {
      last_path_ = file_path;
      Save();
    }
  }
}

void SaveButton::Save() {
  if (last_path_ != "") {
    icosphere_->Save(last_path_);
    std::cout << "Model saved as " << last_path_ << std::endl;
  }
}
