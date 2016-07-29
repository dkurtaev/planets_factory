// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_ACTIONS_VIEW_H_
#define EDITOR_INCLUDE_ACTIONS_VIEW_H_

#include <vector>
#include <string>
#include <map>

#include "include/glview.h"
#include "include/button.h"
#include "include/layout.h"

#include <yaml-cpp/yaml.h>

class SubmenuOpener;
class ActionsView : public GLView {
 public:
  ActionsView(const std::vector<Button*>& buttons, const std::string& config,
              GLView* parent = 0);

  ~ActionsView();

  virtual void Display();

  void SetMenu(const std::vector<Button*>& buttons);

 private:
  static const unsigned kInitButtonRoiWidth = 150;
  static const unsigned kInitButtonRoiHeight = 50;
  // Idents of drawing buttons from ROI borders.
  static const float kLeftIdent = 0.1f;
  static const float kRightIdent = 0.1f;
  static const float kTopIdent = 0.2f;
  static const float kBottomIdent = 0.2f;

  void ParseConfig(const std::string& path,
                   const std::vector<Button*>& buttons);

  void ParseMenu(const YAML::Node& node,
                 const std::map<std::string, Button*>& all_buttons,
                 std::vector<Button*>* menu_buttons);

  std::vector<Button*> buttons_;
  Layout layout_;
  std::vector<Roi> buttons_rois_;
  std::vector<SubmenuOpener*> created_submenus_;
};

// Shell of button for menu implementation.
class SubmenuOpener : public Button {
 public:
  SubmenuOpener(ActionsView* actions_view, const std::string& title);

  virtual void MouseFunc(int button, int state, int x, int y);

  void AddToSubmenu(Button* item);

 private:
  ActionsView* actions_view_;
  std::vector<Button*> submenu_;
};

#endif  // EDITOR_INCLUDE_ACTIONS_VIEW_H_
