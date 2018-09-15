#include "src/libmodel/cpp/skeleton.cpp"
// some very stupid implementation of the virtuals.
#include "src/libmodel/cpp/stubs.cpp"
//
//typedef struct {
//
//} DeviceConfig;
//
//// Address
//typedef struct {
//} Address;

class ArduinoLed : Led {
  public:
    ArduinoLed(int pin) {
      _pin = pin;
    }

    void setup() {
      pinMode(_pin, INPUT);
    }

    virtual void setState(bool on) {
      digitalWrite(_pin, on ? HIGH : LOW);
    }

  private:
    int _pin;
};

StubLeakSensor leakSensor;
ArduinoLed led(13); // 13 is the arduino default led.

void setup() {
  Serial.begin(115200);
  led.setup();
}

void loop() {
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
