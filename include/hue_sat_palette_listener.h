#ifndef INCLUDE_HUE_SAT_PALETTE_LISTENER_H_
#define INCLUDE_HUE_SAT_PALETTE_LISTENER_H_

#include "include/glview_listener.h"

class PaletteView;
class HueSatPaletteListener : public GLViewListener {
 public:
  HueSatPaletteListener(PaletteView* palette_view);

  virtual void MouseMove(int x, int y);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  PaletteView* palette_view_;
};

#endif  // INCLUDE_HUE_SAT_PALETTE_LISTENER_H_
