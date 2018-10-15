#include "button.h"

Button::Button(SystemTime* _systemTime)
{
  //ctor
  systemTime = _systemTime;
  onLongPressListener = nullptr;
  onPressListener = nullptr;
  pressed = false;
  pressedAt = 0;
  longPressHandled = false;
}

Button::~Button()
{
  //dtor
}
void Button::setPressed(bool _pressed)
{
  if (!pressed && _pressed) {
    pressed = _pressed;
    pressedAt = systemTime->nowMillis();
    onPressListener->onPress();
  } else if (pressed && !longPressHandled
             && (systemTime->nowMillis() - pressedAt > SPN_BUTTON_LONG_PRESS_TIME)) {
    longPressHandled = true;
    onLongPressListener->onPress();
  } else if (pressed && !_pressed) {
    // reset
    longPressHandled = false;
    pressedAt = 0;
    pressed = _pressed;
  }
}

void Button::press()
{
  setPressed(true);
  setPressed(false);
}
