#include "systemtime.h"
#include "button.h"
#include "buzzer.h"
#include "led.h"

#define PIN_NO_PIN -1

class RealButton : public Button {
  public:
    RealButton(SystemTime* _systemTime, long longPressTime, int _pin) : Button(_systemTime, longPressTime) {
      pin = _pin;
    }
    void setup() {
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT_PULLUP);
      }
    }

    void update() {
      Button::update();
      if (pin != PIN_NO_PIN) {
        bool pressed = digitalRead(pin) == LOW;
        if (pressed) {
          DEBUG("Button %d pressed", pin);
        }
        setPressed(pressed);
      }
    }

  private:
    int pin;
};


class RealBuzzer : public Buzzer {
  public:
    RealBuzzer(int _pin, int _beepLength) : Buzzer() {
      pin = _pin;
      beepLength = _beepLength;
    }

    void setup() {
      Buzzer::setup();
      if (pin != PIN_NO_PIN) {
        pinMode(pin, OUTPUT);
      }
    }

    void beep() {
      on();
      delay(beepLength);
      off();
    }

    void on() {
      if (pin != PIN_NO_PIN) {
        digitalWrite(pin, HIGH);
      }
    }

    void off() {
      if (pin != PIN_NO_PIN) {
        digitalWrite(pin, LOW);
      }
    }
  private:
    int pin;
    int beepLength;
};


typedef void (*voidFuncPtr)(void); // Create a type to point to a funciton.

class FunctionPressListener : public OnButtonPressListener {
  public:
    FunctionPressListener(voidFuncPtr _callback) {
      callback = _callback;
    }
    void onPress() {
      callback();
    }
  private:
    voidFuncPtr callback;
};

class HardwarePinLed : public Led {
  public:
    HardwarePinLed(int pin) {
      _pin = pin;
    }

    void setup() {
      Led::setup();
      if (_pin != PIN_NO_PIN) {
        pinMode(_pin, OUTPUT);
      }
    }

    void setState(bool on) {
      Led::setState(on);
      if (_pin != PIN_NO_PIN) {
        digitalWrite(_pin, on ? HIGH : LOW);
      }
    }

  private:
    int _pin;
};

class RealSystemTime : public SystemTime {
  public:
    RealSystemTime() : SystemTime() {}

    long nowMillis() {
      return millis();
    }
};
