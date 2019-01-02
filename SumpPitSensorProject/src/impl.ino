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
#include "state.h";

#define PIN_NO_PIN -1
#define PUB_SHUTOFF_STATE "shutoff-valve-state"

class AnalogWaterLevelSensor : public WaterLevelSensor {
  public:
    /**
    constructor
    @param min minimum measurable distance (max measurable water level)
    @param max maximum measurable distance (empty tank)
    */
    AnalogWaterLevelSensor(int _pin, double _min, double _max) {
      min = _min;
      max = _max;
      pin = _pin;
    }
    short measureLevel() {
      if (pin != PIN_NO_PIN) {
        return (int) map(analogRead(pin), (double)min, (double)max, 100.0, 0.0);
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
  private:
    int pin;
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
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT);
      }
    }

    float getVoltage() {
      VoltageSensor::getVoltage();
      if (pin != PIN_NO_PIN) {
        return analogRead(pin);
      } else {
        return 0;
      }
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
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT_PULLUP);
      }
    }
    bool isLeaking() {
      LeakSensor::isLeaking();
      if (pin != PIN_NO_PIN) {
        // PULLUP is enabled, pulled low when water detected
        return digitalRead(pin) == LOW;
      }
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
      if (_pin != PIN_NO_PIN) {
        pinMode(_pin, OUTPUT);
      }
    }

    void setState(bool on) {
      if (_pin != PIN_NO_PIN) {
        digitalWrite(_pin, on ? HIGH : LOW);
      }
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
      if (pin != PIN_NO_PIN) {
        pinMode(pin, OUTPUT);
      }
    }

    void on() {
      if (pin != PIN_NO_PIN) {
        digitalWrite(pin, HIGH);
      }
    }

    void off() {
      //digitalWrite(pin, LOW);
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
      if (pin != PIN_NO_PIN) {
        pinMode(pin, INPUT_PULLUP);
      }
    }

    void update() {
      if (pin != PIN_NO_PIN) {
        bool pressed = digitalRead(pin) == LOW;
        setPressed(pressed);
      }
    }

  private:
    int pin;
};

class RealRpmSensor : public RpmSensor {
  public:
    int getRpm() {
      // TODO: implement rpm detection here.
      return 0;
    }
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
            bank[bankIndex++] = message[i];
          }
          messageIndex += len + 1;
        } else {
          end = true;
        }

        if (!end) {
          y++;
          bankIndex = (y * 20);
        }
      }

      bank[bankSize - 1] = '\0';
      lcd->setCursor(0,0);
      lcd->printstr(bank);
      // fill space
      /*for (int k=line; k<4; ++k) {
        for (int j=0; j<20;++j) {
          lcd->setCursor(k, j);
          lcd->write(' ');
        }
      }
      lcd->printstr(message);
      lcd->setCursor(0,0);
      lcd->write('0' + ((millis() / 100) % 10));*/
    }

    void clear() {
      Display::clear();
      /*for (int i=0; bankSize; ++i) {
        bank[i] = ' ';
      }
      bank[bankSize - 1]='\0';
      lcd->print("bank\0");*/
      lcd->clear();
    }

  private:
    LiquidCrystal_I2C* lcd;
};
