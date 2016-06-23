#include "include/listener_enabler.h"

#include <stdlib.h>
#include <string.h>

#include <string>

static const float kPressedButtonColor[3] = { 0.95f, 0.72f, 0.09f };

ListenerEnabler::ListenerEnabler(std::string text, GLViewListener* listener)
    : Button(text), listener_(listener) {
  memcpy(default_button_color_, button_color_, sizeof(float) * 3);
  if (listener_->IsEnabled()) {
    memcpy(button_color_, kPressedButtonColor, sizeof(float) * 3);
  }
}

void ListenerEnabler::MouseFunc(int button, int state, int x, int y) {
  if (!state) {
    if (listener_->IsEnabled()) {
      listener_->Disable();
      memcpy(button_color_, default_button_color_, sizeof(float) * 3);
    } else {
      listener_->Enable();
      memcpy(button_color_, kPressedButtonColor, sizeof(float) * 3);
    }
  }
}
