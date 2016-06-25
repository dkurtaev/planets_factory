#ifndef INCLUDE_HSV_PALETTE_LISTENERS_H_
#define INCLUDE_HSV_PALETTE_LISTENERS_H_

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

#endif  // INCLUDE_HSV_PALETTE_LISTENERS_H_
