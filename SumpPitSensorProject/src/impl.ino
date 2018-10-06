#include "sumppitnode.h"
#include "sumppitnode.h"
#include "led.h"
#include "leaksensor.h"
#include "spn_config.h"
#include "button.h"
#include "sumppitsensor.h"
#include "HC_SR04.h"
#include "waterlevelsensor.h"
#include "buzzer.h"
#include "application.h"

#define PUB_SHUTOFF_STATE "shutoff-valve-state"

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
      pinMode(pin, INPUT_PULLUP);
    }
    bool isLeaking() {
      LeakSensor::isLeaking();
      // PULLUP is enabled, pulled low when water detected
      return digitalRead(pin) == LOW;
    }
  private:
    int pin;
};

class HardwarePinLed : public Led {
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

class RealSiren : public Buzzer {
  public:
    RealSiren(int _pin) : Buzzer() {
      pin = _pin;
    }

    void setup() {
      pinMode(pin, OUTPUT);
    }

    void on() {
      digitalWrite(pin, HIGH);
    }

    void off() {
      digitalWrite(pin, LOW);
    }
  private:
    int pin;
};

class RealShutoffValve : public ShutoffValve {
  public:
    RealShutoffValve() : ShutoffValve() {
      active = false;
    }

    void setup() {
      //Particle.subscribe(PUB_SHUTOFF_STATE, &RealShutoffValve::myHandler);
    }
    void activate() {
      if (!active && Particle.publish(PUB_SHUTOFF_STATE, "true", PRIVATE | WITH_ACK)) {
        active = true;
      }
    }
    void deactivate() {
      if (!active && Particle.publish(PUB_SHUTOFF_STATE, "false", PRIVATE | WITH_ACK)) {
        active = false;
      }
    }
    bool isActive() {
      return active;
    }

  private:
    int pin;
    bool active;
};

class RealBuzzer : public RealSiren {
  public:
    RealBuzzer(int _pin) : RealSiren(_pin) {

    }
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
    RealButton(SystemTime* _systemTime, int _pin) : Button(_systemTime) {
      pin = _pin;
    }
    void setup() {
      pinMode(pin, INPUT_PULLUP);
    }

    void update() {
      bool pressed = digitalRead(pin) == LOW;
      setPressed(pressed);
    }

  private:
    int pin;
};
