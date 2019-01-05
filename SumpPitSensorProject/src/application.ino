// If you get File not found error for 'localsettings.h', please
// copy the localsettings.h file from SumpPitSensorProject to
// SumpPitSensorProject/src
#include "localsettings.h"

#define PLATFORM_PHOTON 1
#define PLATFORM_ELECTRON 2

// enable cloud by default.
// enter this to disable
// #define CLOUD_ENABLED false
#ifndef CLOUD_ENABLED
#define CLOUD_ENABLED true
#endif

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

// Photon platform from here
#if PLATFORM == PLATFORM_PHOTON

// Electron platform from here
#elif PLATFORM == PLATFORM_ELECTRON
/*#define PIN_LED_GREEN B0
#define PIN_LED_YELLOW B1
#define PIN_LED_RED B2
#define PIN_LEAK_1 C5
#define PIN_LEAK_2 C4
// TODO: rename butons.
#define PIN_BUTTON_1 A0
#define PIN_BUTTON_2 B5
#define PIN_BUTTON_3 B4
#define PIN_BUZZER D2
#define PIN_SIREN B3*/
//#define PIN_WATERLEVEL A0
//#define PIN_PUMP_VOLTAGE_1 A4
//#define PIN_PUMP_VOLTAGE_2 A3
#endif
/////////////////////////
// fallback to no pin when something is not defined.
/////////////////////////
#ifndef PIN_LED_GREEN
#define PIN_LED_GREEN PIN_NO_PIN
#endif
#ifndef PIN_LED_YELLOW
#define PIN_LED_YELLOW PIN_NO_PIN
#endif
#ifndef PIN_LED_RED
#define PIN_LED_RED PIN_NO_PIN
#endif
#ifndef PIN_LEAK_1
#define PIN_LEAK_1 PIN_NO_PIN
#endif
#ifndef PIN_LEAK_2
#define PIN_LEAK_2 PIN_NO_PIN
#endif
#ifndef PIN_BUTTON_1
#define PIN_BUTTON_1 PIN_NO_PIN
#endif
#ifndef PIN_BUTTON_2
#define PIN_BUTTON_2 PIN_NO_PIN
#endif
#ifndef PIN_BUTTON_3
#define PIN_BUTTON_3 PIN_NO_PIN
#endif
#ifndef PIN_BUZZER
#define PIN_BUZZER PIN_NO_PIN
#endif
#ifndef PIN_SIREN
#define PIN_SIREN PIN_NO_PIN
#endif
#ifndef PIN_WATERLEVEL
#define PIN_WATERLEVEL PIN_NO_PIN
#endif
#ifndef PIN_PUMP_VOLTAGE_1
#define PIN_PUMP_VOLTAGE_1 PIN_NO_PIN
#endif
#ifndef PIN_PUMP_VOLTAGE_2
#define PIN_PUMP_VOLTAGE_2 PIN_NO_PIN
#endif
// end of fallback definitions.

//////////////////////////
// minimum measurable water disatance (inches)
#define WATER_DIST_MIN 0
#define WATER_DIST_MAX 3516

#define DISPLAY_I2C_ADDR 0x27
#define DISPLAY_COLS 20
#define DISPLAY_ROWS 4


RealSiren* siren;
RealBuzzer* buzzer;
RealSystemTime* systemTime;
RealLeakSensor* leakSensor;
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
  leakSensor = new RealLeakSensor(PIN_LEAK_1, PIN_LEAK_2);
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
  sensor = new SumpPitSensor(waterLevelSensor, 1, leakSensor, 1, multiPump);
  localView = new LocalView(display, ledRed, ledGreen, ledYellow);
  node = new SumpPitNode(siren, buzzer, localView, sensor, inputs, shutoffValve);
  node->setup();

  if (CLOUD_ENABLED) {
    bool success = Particle.function("arm", armSystem);
    success = Particle.function("disarm", disarmSystem);
    success = Particle.function("startMaintenance", startMaintenance);

  }

  Serial.begin(115200);
}

int armSystem(String extra) {
  node->arm();
  return 0;
}
int disarmSystem(String extra) {
  node->disarm();
  return 0;
}
int startMaintenance(String extra) {
  node->maintenance();
  return 0;
}

int lastStatus = -1;
char statusString[10];
char publishString[256];
// loop() runs over and over again, as quickly as it can execute.
void loop() {

  State* state = node->update();
  if (CLOUD_ENABLED && state->alarmReason != lastStatus && Particle.connected() == 1) {
    statusToString(state->alarmReason, statusString);
    sprintf(publishString,
      "{"
      "'alarm':'%s', 'waterLevel':'%d',"
      "'leakSensor':'%s',"
      "'rpm1':'%d','rpm2':'%d',"
      "'uptime':'%d'"
      "}",

      statusString, waterLevelSensor->measureLevel(),
      leakSensor->isLeaking() ? "true" : "false",
      rpmSensor1->getRpm(), rpmSensor2->getRpm(),
      systemTime->nowMillis()
    );
    Particle.publish("status", publishString);

    lastStatus = state->alarmReason;
  }

  Particle.process();
  Serial.print("-----@");
  Serial.print(systemTime->nowMillis());
  Serial.println("-----");
  Serial.print("pump1Voltage:");
  Serial.println(voltageSensor1->getVoltage());
  Serial.print("pump2Voltage:");
  Serial.println(voltageSensor2->getVoltage());
  Serial.print("pump1Rpm:");
  Serial.println(pump1->getRpm());
  Serial.print("pump2Rpm:");
  Serial.println(pump2->getRpm());
  Serial.print("leakSensor:");
  Serial.println(leakSensor->isLeaking());
  delay(50);
}
