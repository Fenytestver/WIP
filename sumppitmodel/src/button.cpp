#include "button.h"

Button::Button(SystemTime* _systemTime, long _longPressTime)
{
  //ctor
  systemTime = _systemTime;
  longPressTime = _longPressTime;
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

void Button::setup() {

}

bool Button::isPressed() {
  return pressed;
}

void Button::setPressed(bool _pressed)
{
  if (!pressed && _pressed) {
    pressed = _pressed;
    pressedAt = systemTime->nowMillis();
    onPressListener->onPress();
  } else if (pressed && !longPressHandled
             && (systemTime->nowMillis() - pressedAt > longPressTime)) {
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

void Button::update() {

}
