#define PLATFORM_PHOTON 1
#define PLATFORM_ELECTRON 2
// select platform by uncommenting one of the lines below
// #define PLATFORM PLATFORM_PHOTON
// #define PLATFORM PLATFORM_ELECTRON
#ifndef PLATFORM
#error "Platform is not defined, please open application.ino"
#endif

#define DEBUG 0
#include "impl.ino"
#include "sumppitinputs.h"
#include "sumppitnode.h"
#include "application.h"

// main system mode
SYSTEM_MODE(AUTOMATIC);

// set up pins
#if PLATFORM == PLATFORM_PHOTON
#define PIN_LED_GREEN PIN_NO_PIN
#define PIN_LED_YELLOW PIN_NO_PIN
#define PIN_LED_RED PIN_NO_PIN
#define PIN_LEAK_1 D2
#define PIN_LEAK_2 D3
// TODO: rename butons.
#define PIN_BUTTON_1 PIN_NO_PIN
#define PIN_BUTTON_2 PIN_NO_PIN
#define PIN_BUTTON_3 PIN_NO_PIN
#define PIN_BUZZER PIN_NO_PIN
#define PIN_SIREN PIN_NO_PIN
#define PIN_WATERLEVEL PIN_NO_PIN

#define PIN_PUMP_VOLTAGE_1 A4
#define PIN_PUMP_VOLTAGE_2 A3

#elif PLATFORM == PLATFORM_ELECTRON

#define PIN_LED_GREEN B0
#define PIN_LED_YELLOW B1
#define PIN_LED_RED B2
#define PIN_LEAK_1 C5
#define PIN_LEAK_2 C4
// TODO: rename butons.
#define PIN_BUTTON_1 A0
#define PIN_BUTTON_2 B5
#define PIN_BUTTON_3 B4
#define PIN_BUZZER D2
#define PIN_SIREN B3
#define PIN_WATERLEVEL PIN_NO_PIN

#define PIN_PUMP_VOLTAGE_1 A4
#define PIN_PUMP_VOLTAGE_2 A3
#endif

// minimum measurable water disatance (inches)
#define WATER_DIST_MIN 4
#define WATER_DIST_MAX 78

#define DISPLAY_I2C_ADDR 0x27
#define DISPLAY_COLS 20
#define DISPLAY_ROWS 4


RealSiren* siren;
RealBuzzer* buzzer;
RealSystemTime* systemTime;
RealLeakSensor* leakSensor1;
RealLeakSensor* leakSensor2;
HardwarePinLed* ledGreen;
HardwarePinLed* ledYellow;
HardwarePinLed* ledRed;
RealButton* armButton;
RealButton* maintenanceButton;
RealButton* disarmButton;
RealShutoffValve* shutoffValve;
AnalogWaterLevelSensor* waterLevelSensor;
SumpPitInputs* inputs;
RealRpmSensor* rpmSensor1;
RealRpmSensor* rpmSensor2;
RealVoltageSensor* voltageSensor1;
RealVoltageSensor* voltageSensor2;
Pump* pump1;
Pump* pump2;
MultiPump* multiPump;
LcdDisplay* display;
LocalView* localView;

// TODO: 2 leak sensors
SumpPitSensor* sensor;
SumpPitNode* node;

/*
 * Project SumpPitSensorProject
 * Description:
 * Author:
 * Date:
 */

// setup() runs once, when the device is first turned on.
void setup() {
  systemTime = new RealSystemTime();
  display = new LcdDisplay(DISPLAY_I2C_ADDR, DISPLAY_COLS, DISPLAY_ROWS);
  siren = new RealSiren(PIN_SIREN);
  buzzer = new RealBuzzer(PIN_BUZZER);
  leakSensor1 = new RealLeakSensor(PIN_LEAK_1);
  leakSensor2 = new RealLeakSensor(PIN_LEAK_2);
  ledGreen = new HardwarePinLed(PIN_LED_GREEN);
  ledYellow = new HardwarePinLed(PIN_LED_YELLOW);
  ledRed = new HardwarePinLed(PIN_LED_RED);
  armButton = new RealButton(systemTime, PIN_BUTTON_1);
  maintenanceButton = new RealButton(systemTime, PIN_BUTTON_2);
  disarmButton = new RealButton(systemTime, PIN_BUTTON_3);
  shutoffValve = new RealShutoffValve();
  waterLevelSensor = new AnalogWaterLevelSensor(
    PIN_WATERLEVEL, WATER_DIST_MIN, WATER_DIST_MAX);
  inputs = new SumpPitInputs(disarmButton, maintenanceButton, armButton);
  rpmSensor1 = new RealRpmSensor();
  rpmSensor2 = new RealRpmSensor();
  voltageSensor1 = new RealVoltageSensor(PIN_PUMP_VOLTAGE_1);
  voltageSensor2 = new RealVoltageSensor(PIN_PUMP_VOLTAGE_2);
  pump1 = new Pump(systemTime, rpmSensor1, voltageSensor1);
  pump2 = new Pump(systemTime, rpmSensor2, voltageSensor2);
  multiPump = new MultiPump();
  multiPump->addPump(pump1);
  multiPump->addPump(pump2);
  sensor = new SumpPitSensor(waterLevelSensor, 1, leakSensor1, 1, multiPump);
  localView = new LocalView(display);
  node = new SumpPitNode(siren, buzzer, localView, sensor, inputs, shutoffValve);
  node->setup();

  Serial.begin(115200);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  node->update();
  Particle.process();
  delay(100);
}
