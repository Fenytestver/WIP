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
#include "rpmsensor.h"
#include "display.h"
#include "LiquidCrystal_I2C.h"
#include "state.h"
#include "floatswitch.h"

#define PIN_NO_PIN -1
#define PUB_SHUTOFF_STATE "shutoffValve"

class AnalogWaterLevelSensor : public WaterLevelSensor {
  public:
    /**
    constructor
    @param min minimum measurable distance (max measurable water level)
    @param max maximum measurable distance (empty tank)
    @param _waterLowIn minimum water in inches
    @param _waterHighIn maximum water in inches
    */
    AnalogWaterLevelSensor(int _pin, double _min, double _max,
      int _waterLowIn, int _waterHighIn, int _waterPercentHigh)
        : WaterLevelSensor(_waterLowIn, _waterHighIn, _waterPercentHigh){
      min = _min;
      max = _max;
      pin = _pin;
      waterPercentHigh = _waterPercentHigh;
    }
    short measureLevel() {
      if (pin != PIN_NO_PIN) {
        return (int) map(analogRead(pin), (double)min, (double)max, (double)SPN_WATER_LOW, (double)waterPercentHigh);
      } else {
        return SPN_WATER_LOW + 1;
      }
    }
    int checkState() {
      return SPN_ALARM_NO_ALARM;
    }
    void setup() {
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT);
      }
    }

    int readRaw() {
      if (pin != PIN_NO_PIN) {
        return analogRead(pin);
      } else {
        return SPN_WATER_LOW + 1;
      }
    }
  private:
    int pin;
    int min;
    int max;
    int waterPercentHigh;
};

class RealVoltageSensor : public VoltageSensor {
  public:
    RealVoltageSensor(int _pin) : VoltageSensor() {
      pin = _pin;
    }

    void setup() {
      VoltageSensor::setup();
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT_PULLUP);
      }
    }

    float getVoltage() {
      VoltageSensor::getVoltage();
      if (pin != PIN_NO_PIN) {
        return digitalRead(pin) == LOW;
      } else {
        return 0;
      }
    }


  private:
    int pin;
};

class RealLeakSensor : virtual public LeakSensor {
  public:
    RealLeakSensor(int _pin, int _pin2, int _pin3) : LeakSensor() {
      pin = _pin;
      pin2 = _pin2;
      pin3 = _pin3;
    }
    void setup() {
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT_PULLUP);
      }
      if (pin2 != PIN_NO_PIN) {
        pinMode(pin2, INPUT_PULLUP);
      }
      if (pin3 != PIN_NO_PIN) {
        pinMode(pin3, INPUT_PULLUP);
      }
    }
    bool isLeaking() {
      LeakSensor::isLeaking();
      return readPin(pin) || readPin(pin2) || readPin(pin3);
    }
  private:
    int pin;
    int pin2;
    int pin3;
    bool readPin(int pin) {
      if (pin != PIN_NO_PIN) {
        // PULLUP is enabled, pulled low when water detected
        return digitalRead(pin) == LOW;
      } else {
        return false;
      }
    }
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

class RealSiren : public Siren {
  public:
    RealSiren(SystemTime* _systemTime, int _pin) : Siren() {
      pin = _pin;
      turnedOn = false;
      systemTime = _systemTime;
    }

    void setup() {
      Siren::setup();
      if (pin != PIN_NO_PIN) {
        pinMode(pin, OUTPUT);
      }
    }

    void on() {
      Siren::on();
      turnedOn = true;
    }

    void off() {
      turnedOn = false;
      Siren::off();
      if (pin != PIN_NO_PIN) {
        digitalWrite(pin, LOW);
      }
    }
    void update() {
      Siren::update();
      if (pin != PIN_NO_PIN && turnedOn) {
        digitalWrite(pin, isTime() ? HIGH : LOW);
      }
    }
  private:
    int pin;
    bool turnedOn;
    SystemTime* systemTime;
    bool isTime() {
      long now = systemTime->nowMillis();
      return ((now / 1000) % 2) == 0;
    }
};

class RealShutoffValve : public ShutoffValve {
  public:
    RealShutoffValve() : ShutoffValve() {
      active = false;
    }

    void setup() {
    }

    void activate() {
      if (!active && Particle.publish(PUB_SHUTOFF_STATE, "true", WITH_ACK)) {
        active = true;
      }
    }
    void deactivate() {
      if (active) {
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

class RealBuzzer : public Buzzer {
  public:
    RealBuzzer(int _pin) : Buzzer() {
      pin = _pin;
    }

    void setup() {
      Buzzer::setup();
      if (pin != PIN_NO_PIN) {
        pinMode(pin, OUTPUT);
      }
    }

    void beep() {
      on();
      delay(50);
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
        Serial.println(pin);
        bool pressed = digitalRead(pin) == LOW;
        setPressed(pressed);
      }
    }

  private:
    int pin;
};

class RealRpmSensor : public RpmSensor {
  public:
    RealRpmSensor(int _pin, SystemTime* _systemTime) {
      pin = _pin;
      systemTime = _systemTime;
      count = 0;
      lastRpm = 0;
    }

    void setup() {
      RpmSensor::setup();
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT_PULLUP);
      }
      timestamp = systemTime->nowMillis();
    }

    int getRpm() {
      long lastupdate = timestamp;
      long currentCount = count;
      long now = systemTime->nowMillis();
      // not the same second
      if (now - lastupdate > 500L) {
        count = 0;
        timestamp = now;
        lastRpm = (int)(((float)currentCount) / (((float)(now - lastupdate) / 1000.0)));
      }
      return lastRpm;
    }
    void trigger() {
      count++;
    }
  private:
    int pin;
    SystemTime* systemTime;
    volatile long count;
    long timestamp;
    int lastRpm;

};

class LcdDisplay : public Display {
  private:
    const int bankSize = 20*4 + 1;
    char bank[20*4 + 1];
  public:
    LcdDisplay(int address, int cols, int rows) : Display() {
      lcd = new LiquidCrystal_I2C(address, cols, rows);
      for (int i=0; i<bankSize; ++i) {
        bank[i] = ' ';
      }
      bank[bankSize+1] = '\0';
    }

    void setup() {
      Display::setup();
      lcd->init();
      lcd->backlight();
      lcd->clear();
    }
    char* getBank() {
      return bank;
    }
    void show(State state) {

    }

    void displayMessage(char* message) {
      Display::displayMessage(message);
      //lcd->setCursor(0,0);
      //lcd->write('a');
    //  return;
      char c = message[0];
      int messageIndex = 0;
      int bankIndex = 0;
      int x = 0;
      int y = 0;
      int countX = 20;
      int countY = 4;
      for (int i=0; i < bankSize; ++i) {
        bank[i] = ' ';
      }
      String str = String(message);

      bool end = false;
      while (!end && y < countY && bankIndex < bankSize) {
        int nextNL = str.indexOf('\n', messageIndex);
        if (nextNL == -1) {
            nextNL = str.indexOf('\0', messageIndex);
        }

        if (nextNL != -1) {
          int len = nextNL - messageIndex;

          for (int i = messageIndex; i < nextNL; ++i) {
            if (i < messageIndex + 20) {
              bank[bankIndex++] = message[i];
              // TODO: run the loop until minimum of nextNL and 20(display size).
            }
          }
          messageIndex += len + 1;
        } else {
          end = true;
        }

        if (!end) {
          y++;
          // TODO: ugly code.
          // my lcd line order is ACBD, fix that here.
          switch (y) {
            case 1:
              bankIndex = (2 * 20);
              break;
            case 2:
              bankIndex = (1 * 20);
              break;
            case 0:
            case 3:
            default:
              bankIndex = (y * 20);
              break;
          }

        }
      }

      bank[bankSize - 1] = '\0';
      lcd->setCursor(0,0);
      lcd->printstr(bank);
    }

    void clear() {
      Display::clear();
      for (int i=0; bankSize; ++i) {
        bank[i] = ' ';
      }
      bank[bankSize - 1]='\0';
      lcd->clear();
    }

  private:
    LiquidCrystal_I2C* lcd;
};

class RealFloatSwitch : virtual public FloatSwitch, private RealLeakSensor {
  public:
    RealFloatSwitch(int pin1, int pin2) : RealLeakSensor(pin1, pin2, PIN_NO_PIN), FloatSwitch() {

    }

    void setup() {
      RealLeakSensor::setup();
    }

    bool isTriggered() {
      return RealLeakSensor::isLeaking();
    }

};
