#ifndef INCLUDE_VALUE_PALETTE_LISTENER_H_
#define INCLUDE_VALUE_PALETTE_LISTENER_H_

#include "include/glview_listener.h"

class PaletteView;
class ValuePaletteListener : public GLViewListener {
 public:
  ValuePaletteListener(PaletteView* palette_view);

  virtual void MouseMove(int x, int y);

  virtual void MouseFunc(int button, int state, int x, int y);

 private:
  PaletteView* palette_view_;
};

#endif  // INCLUDE_VALUE_PALETTE_LISTENER_H_
