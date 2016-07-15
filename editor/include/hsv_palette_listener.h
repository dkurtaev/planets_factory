// Copyright © 2016 Dmitry Kurtaev. All rights reserved.
// e-mail: dmitry.kurtaev@gmail.com
#ifndef EDITOR_INCLUDE_HSV_PALETTE_LISTENER_H_
#define EDITOR_INCLUDE_HSV_PALETTE_LISTENER_H_

#include "include/glview_listener.h"

enum Target { HUESAT, VALUE };

class PaletteView;
class HSVPaletteListener : public GLViewListener {
 public:
  HSVPaletteListener(Target target, PaletteView* palette_view);

  virtual void MouseMove(int x, int y);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  Target target_;
  PaletteView* palette_view_;
};

#endif  // EDITOR_INCLUDE_HSV_PALETTE_LISTENER_H_
