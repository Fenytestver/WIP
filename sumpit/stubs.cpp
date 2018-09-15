#ifndef stubs_cpp
#define stubs_cpp

#include "skeleton.cpp"

///
// Very dumb, non-blocking, checkable implementations.
///
class StubButton : Button {
  public:
    void press() {
      if (_onPressListener) {
        _onPressListener->onPress();
      }
    }
};

class StubLeakSensor : LeakSensor {
  public:
    void setLeaking(bool leaking) {
      _leaking = leaking;
    }
    virtual bool isLeaking() {
      return _leaking;
    }
  private:
    bool _leaking = false;
};

class StubLed : Led {
  public:
    virtual void setState(bool on) {
      _on = on;
    }
    bool isOn() {
      return _on;
    }
  private:
    bool _on = false;
};

class StubBuzzer : Buzzer {
  public:
    virtual void beep() {
      buzzer++;
    }
    int getBuzzer() {
      return buzzer;
    }

  private:
    int buzzer = 0;
};
#endif
