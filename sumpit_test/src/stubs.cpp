#ifndef stubs_cpp
#define stubs_cpp

#include "sumppitsensor.h"
#include "siren.h"
#include "button.h"
#include "buzzer.h"
#include "leaksensor.h"
#include "led.h"
#include "display.h"
#include "debug_util.h"
#include "string.h"

///
// Very dumb, non-blocking, checkable implementations.
///
class StubLed : public Led {
  public:
    StubLed(char* _name) : Led() {
      name = _name;
    }
    virtual void setState(bool on) {
      _on = on;
      //cout << "Led:" << name << " turned " << (on ? "on" : "off");
    }
    bool isOn() {
      return _on;
    }
  private:
    char* name;
    bool _on = false;
};

class StubSiren : public Siren {
  public:
    /** Turn on the alarm. */
    virtual void on() {
    }
    /** Turn off the alarm. */
    virtual void off() {
    }
};
class StubButton : public Button {
  public:
    void press() {
      if (_onPressListener) {
        _onPressListener->onPress();
      }
    }
};

class StubLeakSensor : public LeakSensor {
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

class StubDisplay : public Display {
  public:
    void displayMessage(char* _message)
    {
      message = _message;
    }

    void clear()
    {
      message = 0;
    }
  private:
    char* message;
};

class StubWaterLevelSensor : public WaterLevelSensor {
  public:
    virtual short measureLevel() {
      return level;
    }
    void setLevel(short _level) {
      level = _level;
    }
  private:
    short level = 0;
};
class StubBuzzer : public Buzzer {
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

class StubRpmSensor : public RpmSensor {
  public:
    void setRpm(int _rpm) {
      rpm = _rpm;
    }
    virtual int getRpm() {
      return rpm;
    }
  private:
    int rpm;
};

class StubVoltageSensor : public VoltageSensor{
  public:
    void setVoltage(float _voltage) {
      voltage = _voltage;
    }
    virtual float getVoltage() {
      return voltage;
    }
  private:
    int voltage;
};

class StubPump : public Pump {
  public:
    StubPump(StubRpmSensor* _rpmSensor, StubVoltageSensor* _voltageSensor) :
      Pump(_rpmSensor, _voltageSensor) {
    }
    virtual void turnOn() {
      turnedOn = true;
    }
    virtual void turnOff() {
      turnedOn = false;
    }
    /** gives the on/off state of the pump, as we requested. */
    virtual bool isTurnedOn() {
      return turnedOn;
    }

  private:
    bool turnedOn = false;
};


#endif
