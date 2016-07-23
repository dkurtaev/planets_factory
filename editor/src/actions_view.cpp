// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#include "include/actions_view.h"

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

#include <GL/freeglut.h>
#include <glog/logging.h>

ActionsView::ActionsView(const std::vector<Button*>& buttons,
                         const std::string& config, GLView* parent)
  : GLView(1, 1, "Actions", parent, 0, 0) {
  AddListener(&layout_);
  ParseConfig(config, buttons);
}

ActionsView::~ActionsView() {
  const unsigned n_submenus = created_submenus_.size();
  for (unsigned i = 0; i < n_submenus; ++i) {
    delete created_submenus_[i];
  }
}

void ActionsView::Display() {
  glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, display_width_, display_height_, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

  const unsigned n_buttons = buttons_.size();
  unsigned top, left, right, botttom;
  for (unsigned i = 0; i < n_buttons; ++i) {
    glClear(GL_STENCIL_BUFFER_BIT);

    buttons_rois_[i].Get(display_width_, display_height_, &left, &right, &top,
                         &botttom);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glBegin(GL_QUADS);
      glVertex2i(left, top);
      glVertex2i(left, botttom);
      glVertex2i(right, botttom);
      glVertex2i(right, top);
    glEnd();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glPushMatrix();
    glTranslatef((left + right) / 2, (top + botttom) / 2, 0);
    buttons_[i]->Display((right - left) / 2, (botttom - top) / 2);
    glPopMatrix();
  }
  glDisable(GL_STENCIL_TEST);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glutSwapBuffers();
}

void ActionsView::ParseConfig(const std::string& path,
                              const std::vector<Button*>& buttons) {
  std::ifstream file(path.c_str());
  CHECK(file.is_open());

  std::string str;
  file >> str;
  CHECK_EQ(str, "<menu='actions_view'>");

  std::map<std::string, Button*> all_buttons;
  const unsigned n_buttons = buttons.size();
  for (unsigned i = 0; i < n_buttons; ++i) {
    all_buttons[buttons[i]->GetText()] = buttons[i];
  }

  std::vector<Button*> top_menu_buttons;
  ParseMenu(&file, all_buttons, &top_menu_buttons);
  SetMenu(top_menu_buttons);
  file.close();
}

void ActionsView::ParseMenu(std::ifstream* ifs,
                            const std::map<std::string, Button*>& all_buttons,
                            std::vector<Button*>* menu_buttons) {
  static const char kPseudoWhitespace = '+';
  std::string str;
  std::map<std::string, Button*>::const_iterator it;
  std::vector<SubmenuOpener*> found_submenus;
  std::vector<Button*> submenu_buttons;
  unsigned n_buttons;

  *ifs >> str;
  while (str != "</menu>" && !ifs->eof()) {
    if (str == "<button>") {
      *ifs >> str;
      std::replace(str.begin(), str.end(), kPseudoWhitespace, ' ');
      it = all_buttons.find(str);
      CHECK(it != all_buttons.end()) << "Button " << str << " not presented";
      menu_buttons->push_back(it->second);
      *ifs >> str;
      CHECK_EQ(str, "</button>");
    } else if (str.find("<menu='") != -1) {
      // Substring is button name: <menu='Show/Hide'> for example.
      str = str.substr(str.find("<menu='") + 7, str.length() - 9);
      std::replace(str.begin(), str.end(), kPseudoWhitespace, ' ');
      SubmenuOpener* submenu = new SubmenuOpener(this, str);
      submenu_buttons.clear();
      ParseMenu(ifs, all_buttons, &submenu_buttons);
      n_buttons = submenu_buttons.size();
      for (unsigned i = 0; i < n_buttons; ++i) {
        submenu->AddToSubmenu(submenu_buttons[i]);
      }
      menu_buttons->push_back(submenu);
      found_submenus.push_back(submenu);
      created_submenus_.push_back(submenu);
    }
    *ifs >> str;
  }
  CHECK_EQ(str, "</menu>");

  // Back button.
  if (!found_submenus.empty()) {
    SubmenuOpener* back_button = new SubmenuOpener(this, "<--Back");
    created_submenus_.push_back(back_button);

    n_buttons = menu_buttons->size();
    for (unsigned i = 0; i < n_buttons; ++i) {
      back_button->AddToSubmenu(menu_buttons->operator[](i));
    }

    const unsigned n_submenus = found_submenus.size();
    for (unsigned i = 0; i < n_submenus; ++i) {
      found_submenus[i]->AddToSubmenu(back_button);
    }
  }
}

void ActionsView::SetMenu(const std::vector<Button*>& buttons) {
  layout_.Clear();
  buttons_rois_.clear();
  buttons_.clear();

  const unsigned n_buttons = buttons.size();
  const float roi_height = 1.0f / n_buttons;
  for (unsigned i = 0; i < n_buttons; ++i) {
    Roi roi(kLeftIdent, 1 - kRightIdent, (i + kTopIdent) * roi_height,
            (i + 1 - kBottomIdent) * roi_height);
    layout_.AddListener(buttons[i], roi);
    buttons_rois_.push_back(roi);
    buttons_.push_back(buttons[i]);
  }
  glutSetWindow(window_handle_);
  glutReshapeWindow(kInitButtonRoiWidth, kInitButtonRoiHeight * buttons.size());
  glutPostRedisplay();
}

SubmenuOpener::SubmenuOpener(ActionsView* actions_view,
                             const std::string& title)
  : Button(title), actions_view_(actions_view) {}

void SubmenuOpener::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    actions_view_->SetMenu(submenu_);
  }
}

void SubmenuOpener::AddToSubmenu(Button* item) {
  submenu_.push_back(item);
}
