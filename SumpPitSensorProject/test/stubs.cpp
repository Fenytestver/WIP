#include <iostream>
#include "string.h"

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
#include "systemtime.h"
#include "shutoffvalve.h"

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
      SPN_DBG_STUB << "Led:" << name << " turned " << (on ? "on" : "off")<< endl;
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
      SPN_DBG_STUB << "Siren turned on"<<endl;
      sirenOn = true;
    }
    /** Turn off the alarm. */
    virtual void off() {
      SPN_DBG_STUB << "Siren turned off"<<endl;
      sirenOn = false;
    }
    bool isOn() {
      return sirenOn;
    }
  private:
    bool sirenOn;
};
class StubButton : public Button {
  public:
    StubButton(SystemTime* systemTime) : Button(systemTime) {
    }
    void press() {
      SPN_DBG_STUB << "Button" << " pressed."<< endl;
      if (onPressListener) {
        onPressListener->onPress();
      }
    }
    void setPressed(bool pressed) {
      Button::setPressed(pressed);
    }
};

class StubLeakSensor : public LeakSensor {
  public:
    void setLeaking(bool leaking) {
      SPN_DBG_STUB << "Leak sensor set value to: " << (leaking ? "leaking" : "not leaking") << endl;
      _leaking = leaking;
    }

    virtual bool isLeaking() {
      LeakSensor::isLeaking();
      return _leaking;
    }
  private:
    bool _leaking = false;
};

class StubDisplay : public Display {
  public:
    void displayMessage(char* _message)
    {
      SPN_DBG_STUB << "Display:" << endl;
      debout(_message);
      SPN_DBG_STUB << endl;
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
      SPN_DBG_STUB << "Set water level to " << level << endl;
    }
  private:
    short level = 0;
};
class StubBuzzer : public Buzzer {
  public:
    virtual void beep() {
      SPN_DBG_STUB << "Beep!" << endl;
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
      SPN_DBG_STUB << "RPM set to: " << _rpm << endl;
      rpm = _rpm;
    }
    virtual int getRpm() {
      return rpm;
    }
  private:
    int rpm;
};

class StubVoltageSensor : public VoltageSensor {
  public:
    StubVoltageSensor() : VoltageSensor() {
      voltage = 0;
    }
    void setVoltage(float _voltage) {
      SPN_DBG_STUB << "Stub voltage set to: " << _voltage << endl;
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
    StubPump(SystemTime* _systemTime, StubRpmSensor* _rpmSensor, StubVoltageSensor* _voltageSensor) :
      Pump(_systemTime, _rpmSensor, _voltageSensor) {
    }
    virtual void turnOn() {
      SPN_DBG_STUB << "Pump turned on." << endl;
      turnedOn = true;
    }
    virtual void turnOff() {
      SPN_DBG_STUB << "Pump turned off." << endl;
      turnedOn = false;
    }
    /** gives the on/off state of the pump, as we requested. */
    virtual bool isTurnedOn() {
      return turnedOn;
    }

  private:
    bool turnedOn = false;
};

class StubSystemTime : public SystemTime {

  public:

    StubSystemTime() {
      now = 0L;
    }
    long nowMillis() {
      return now;
    }
    void setTime(long _now) {
      SPN_DBG_STUB << "Set time to " << now << endl;
      now = _now;
    }
    void addTime(long delta) {
      now += delta;
      SPN_DBG_STUB << "Move time by " << delta << " to " << now << endl;
    }
  private:
    long now = 0;
};

class StubShutoffValve : public ShutoffValve {
  public:
    StubShutoffValve() : ShutoffValve() {
      active = false;
    }
    void activate() {
      if (!isActive()) {
        active = true;
        SPN_DBG_STUB << "Shut off valve activated" << endl;
      }
    }
    void deactivate() {
      if (isActive()) {
        active = false;
        SPN_DBG_STUB << "Shut off valve deactivated" << endl;
      }
    }
    virtual bool isActive() {
      return active;
    }
  private:
    bool active = false;
};
#endif
