#include "sumppitnode.h"
#include "sumppitnode.h"
#include "led.h"
#include "leaksensor.h"

#define PLATFORM_PHOTON 1
#define PLATFORM PLATFORM_PHOTON

#if PLATFORM == PLATFORM_PHOTON

#define LED_PIN D7

#endif

class StubLeakSensor : public LeakSensor {
  public:
    void setLeaking(bool leaking) {
      _leaking = leaking;
    }

    bool isLeaking() {
      LeakSensor::isLeaking();
      return _leaking;
    }
  private:
    bool _leaking = false;
};

class HardwarePinLed : Led {
  public:
    HardwarePinLed(int pin) {
      _pin = pin;
    }

    void setup() {
      pinMode(_pin, OUTPUT);
    }
	
	virtual void setState(bool on);

  private:
    int _pin;
};

void HardwarePinLed::setState(bool on) {
  digitalWrite(_pin, on ? HIGH : LOW);
}

StubLeakSensor leakSensor;
HardwarePinLed led(LED_PIN); // 13 is the arduino default led.

/*
 * Project SumpPitSensorProject
 * Description:
 * Author:
 * Date:
 */

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(112500);
  led.setup();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  bool leak = leakSensor.isLeaking();
  Serial.print("Is leaking = ");
  Serial.println(leak);
  // show the leak state
  led.setState(leak);

  // ---> test mode operations, changing state.
  leakSensor.setLeaking(!leakSensor.isLeaking());
  // wait a bit because, why not?
  delay(1000);
}