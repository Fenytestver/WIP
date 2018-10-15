#define DEBUG 0
#include "impl.ino"
#include "sumppitinputs.h"
#include "sumppitnode.h"
#include "application.h"

#define PLATFORM_ELECTRON 1
#define PLATFORM PLATFORM_ELECTRON

#if PLATFORM == PLATFORM_ELECTRON

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
#define PIN_WATERLEVEL_TRIG C3
#define PIN_WATERLEVEL_ECHO C2

#define PIN_PUMP_VOLTAGE_1 A4
#define PIN_PUMP_VOLTAGE_2 A3
// minimum measurable water disatance (inches)
#define WATER_DIST_MIN 4
#define WATER_DIST_MAX 78
#endif

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
UltrasonicWaterLevelSensor* waterLevelSensor;
SumpPitInputs* inputs;
RealRpmSensor* rpmSensor1;
RealRpmSensor* rpmSensor2;
RealVoltageSensor* voltageSensor1;
RealVoltageSensor* voltageSensor2;
Pump* pump1;
Pump* pump2;
MultiPump* multiPump;
LcdDisplay* display;

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
  display = new LcdDisplay();
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
  waterLevelSensor = new UltrasonicWaterLevelSensor(
    PIN_WATERLEVEL_TRIG, PIN_WATERLEVEL_ECHO, WATER_DIST_MIN, WATER_DIST_MAX);
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
  node = new SumpPitNode(siren, buzzer, display, sensor, inputs, shutoffValve);
  node->setup();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  node->update();
  delay(10);
}
