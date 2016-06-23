#include "include/change_color_button.h"

#include <iostream>

ChangeColorButton::ChangeColorButton()
  : Button("Change color") {}

void ChangeColorButton::MouseFunc(int button, int state, int x, int y) {
  std::cout << "Change color!" << std::endl;
}
