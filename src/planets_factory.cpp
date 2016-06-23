#include <vector>

#include "include/planet_view.h"
#include "include/actions_view.h"
#include "include/glview_listener.h"
#include "include/button.h"
#include "include/listener_enabler.h"
#include "include/change_color_button.h"
#include "include/palette_view.h"

#include <GL/freeglut.h>

int main(int argc, char** argv) {
  SphericalCS identity_cs;
  SphericalCS camera_cs(20, 0, 80, 0, &identity_cs);
  CameraMover camera_mover(&camera_cs);
  Icosphere icosphere(4);

  std::vector<Point3f*>* vertices = new std::vector<Point3f*>();
  icosphere.GetVertices(vertices);
  VerticesColorizer vertices_colorizer(vertices);

  PlanetView planet_view(&icosphere, &camera_cs, &camera_mover,
                         &vertices_colorizer);

  std::vector<Button*> buttons;
  buttons.push_back(new ListenerEnabler("Camera", &camera_mover));
  buttons.push_back(new ListenerEnabler("Color", &vertices_colorizer));
  buttons.push_back(new ChangeColorButton());
  ActionsView actions_view(buttons, &planet_view);
  PaletteView palette_view;

  glutMainLoop();
  return 0;
}
