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
#include <iostream>

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
      std::cout << "Led:" << name << " turned " << (on ? "on" : "off")<< std::endl;
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
      sirenOn = true;
    }
    /** Turn off the alarm. */
    virtual void off() {
      std::cout << "Siren turned off"<<std::endl;
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
    void press() {
      std::cout << "Button" << " pressed."<< std::endl;
      if (_onPressListener) {
        _onPressListener->onPress();
      }
    }
};

class StubLeakSensor : public LeakSensor {
  public:
    void setLeaking(bool leaking) {
      std::cout << "Leak sensor set value to: " << (leaking ? "leaking" : "not leaking") << std::endl;
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
      std::cout << "Display:" << message << std::endl;
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
      std::cout << "Beep!" << std::endl;
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
      std::cout << "RPM set to: " << _rpm << std::endl;
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
      std::cout << "Pump voltage set to: " << _voltage << std::endl;
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
      std::cout << "Pump turned on." << std::endl;
      turnedOn = true;
    }
    virtual void turnOff() {
      std::cout << "Pump turned off." << std::endl;
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
