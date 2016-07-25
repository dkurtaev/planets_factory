// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/save_load_buttons.h"

#include <string>
#include <iostream>

#include <QFileDialog>
#include <QObject>
#include <QString>

SaveButton::SaveButton(const Icosphere* icosphere,
                       const TextureColorizer* texture_colorizer)
  : Button("Save"), icosphere_(icosphere), last_model_path_(""),
    last_texture_path_(""), texture_colorizer_(texture_colorizer) {}

void SaveButton::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    // Mesh.
    QString title = QObject::tr("Save model");
    QString ext = QObject::tr("All files (*)");
    QString q_file_path =
        QFileDialog::getSaveFileName(0, title, "", ext, 0,
                                     QFileDialog::DontUseNativeDialog);
    std::string model_file_path = q_file_path.toStdString();
    if (model_file_path != "") {
      last_model_path_ = model_file_path;
    }
    // Texture.
    title = QObject::tr("Save texture");
    ext = QObject::tr("Image Files (*.png *.jpg *.bmp)");
    q_file_path =
        QFileDialog::getSaveFileName(0, title, "", ext, 0,
                                     QFileDialog::DontUseNativeDialog);
    std::string texture_file_path = q_file_path.toStdString();
    if (texture_file_path != "") {
      last_texture_path_ = texture_file_path;
    }
    if (model_file_path != "" || texture_file_path != "") {
      Save();
    }
  }
}

void SaveButton::Save() const {
  if (last_model_path_ != "") {
    icosphere_->Save(last_model_path_);
    std::cout << "Model saved as " << last_model_path_ << std::endl;
  }
  if (last_texture_path_ != "") {
    texture_colorizer_->ExportTexture(last_texture_path_);
    std::cout << "Texture saved as " << last_texture_path_ << std::endl;
  }
}

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
