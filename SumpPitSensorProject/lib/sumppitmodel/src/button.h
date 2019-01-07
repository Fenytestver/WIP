#ifndef BUTTON_H
#define BUTTON_H
#include "onbuttonpresslistener.h"
#include "systemtime.h"

#define SPN_BUTTON_LONG_PRESS_TIME 30000

class Button
{
  public:
    /** Default constructor */
    Button(SystemTime* _systemTime);
    /** Default destructor */
    virtual ~Button();
    virtual void setup();
    void setOnButtonPressListener(OnButtonPressListener* _onPressListener) {
      onPressListener = _onPressListener;
    }

    void setOnButtonLongPressListener(OnButtonPressListener* _onLongPressListener) {
      onLongPressListener = _onLongPressListener;
    }

    virtual void press();
    virtual void update();
  protected:
    OnButtonPressListener* onPressListener;
    OnButtonPressListener* onLongPressListener;
    void setPressed(bool _pressed);
  private:
    bool pressed;
    long pressedAt;
    bool longPressHandled;
    SystemTime* systemTime;
};

#endif // BUTTON_H
