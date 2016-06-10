#include "include/planet_view.h"
#include "include/actions_view.h"
#include "include/glview_listener.h"
#include "include/button.h"

#include <GL/freeglut.h>

int main(int argc, char** argv) {
  PlanetView planet_view;

  std::vector<std::string> actions_titles;
  std::vector<GLViewListener*> buttons;
  actions_titles.push_back("first action");
  actions_titles.push_back("second action");
  buttons.push_back(new Button("first button pressed"));
  buttons.push_back(new Button("second button pressed"));
  ActionsView actions_view(buttons, actions_titles);
  
  glutMainLoop();
  return 0;
}
