#include "sumppitnode.h"
#include "sumppitnode.h"
#include "led.h"
#include "leaksensor.h"
#include "spn_config.h"
#include "button.h"
#include "sumppitsensor.h"
#include "HC_SR04.h"
#include "waterlevelsensor.h"

class UltrasonicWaterLevelSensor : public WaterLevelSensor {
  public:
    /**
    constructor
    @param min minimum measurable distance (max measurable water level)
    @param max maximum measurable distance (empty tank)
    */
    UltrasonicWaterLevelSensor(int trigPin, int echoPin, double _min, double _max) {
      rangefinder = new HC_SR04(trigPin, echoPin);
      min = _min;
      max = _max;
    }
    short measureLevel() {
      double range = rangefinder->getDistanceInch();
      // minimum distance 100% full
      // maxumum distance 0% empty
      return (int) map(range, (double)min, (double)max, 100.0, 0.0);
    }
    int checkState() {
      return SPN_ALARM_NO_ALARM;
    }
    void setup() {

    }
  private:
    HC_SR04* rangefinder;
    int min;
    int max;

};

class RealVoltageSensor : public VoltageSensor {
  public:
    RealVoltageSensor(int _pin) : VoltageSensor() {
      pin = _pin;
    }

    void setup() {
      VoltageSensor::setup();
      pinMode(pin, INPUT);
    }

    float getVoltage() {
      VoltageSensor::getVoltage();
      return analogRead(pin);
    }


  private:
    int pin;
};

class RealLeakSensor : public LeakSensor {
  public:
    RealLeakSensor(int _pin) : LeakSensor() {
      pin = _pin;
    }
    void setup() {
      pinMode(pin, INPUT);
    }
    bool isLeaking() {
      LeakSensor::isLeaking();
      return digitalRead(pin);
    }
  private:
    int pin;
};

class HardwarePinLed : Led {
  public:
    HardwarePinLed(int pin) {
      _pin = pin;
    }

    void setup() {
      pinMode(_pin, OUTPUT);
    }

    void setState(bool on) {
      digitalWrite(_pin, on ? HIGH : LOW);
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

class RealButton : public Button {
  public:
    RealButton(int _pin) : Button() {
      pin = _pin;
    }
    void setup() {
      pinMode(pin, INPUT_PULLUP);
    }
  private:
    int pin;
};
