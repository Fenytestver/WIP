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

#ifndef SIM_3RD_PARTY
#define SIM_3RD_PARTY false
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
#ifndef PIN_LEAK_3
#define PIN_LEAK_3 PIN_NO_PIN
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
#ifndef PIN_PUMP_RPM_1
#define PIN_PUMP_RPM_1 PIN_NO_PIN
#endif
#ifndef PIN_PUMP_RPM_2
#define PIN_PUMP_RPM_2 PIN_NO_PIN
#endif

#ifndef PIN_FLOAT_SWITCH_1
#define PIN_FLOAT_SWITCH_1 PIN_NO_PIN
#endif

#ifndef PIN_FLOAT_SWITCH_2
#define PIN_FLOAT_SWITCH_2 PIN_NO_PIN
#endif

#ifndef SYNC_ENABLED
#define SYNC_ENABLED false
#endif
#ifndef SYNC_PERIOD_MIN
#define SYNC_PERIOD_MIN 30000L
#endif
#ifndef SYNC_PERIOD_MAX
#define SYNC_PERIOD_MAX 600000L
#endif

#ifndef SPN_BUTTON_LONG_PRESS_TIME
#define SPN_BUTTON_LONG_PRESS_TIME 30000
#endif

// end of fallback definitions.

//////////////////////////
// minimum measurable water disatance (inches)
#define WATER_DIST_MIN 0
#define WATER_DIST_MAX 3516

#define DISPLAY_I2C_ADDR 0x27
#define DISPLAY_COLS 20
#define DISPLAY_ROWS 4

#define EEPROM_DEVICE_ID 1

int deviceId;
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
RealFloatSwitch* floatSwitch;

// TODO: 2 leak sensors
SumpPitSensor* sensor;
SumpPitNode* node;
FuelGauge fuel;

/*
 * Project SumpPitSensorProject
 * Description:
 * Author:
 * Date:
 */

// setup() runs once, when the device is first turned on.
void setup() {
  deviceId = -1;
  EEPROM.get(EEPROM_DEVICE_ID, deviceId);
  systemTime = new RealSystemTime();
  display = new LcdDisplay(DISPLAY_I2C_ADDR, DISPLAY_COLS, DISPLAY_ROWS);
  siren = new RealSiren(PIN_SIREN);
  buzzer = new RealBuzzer(PIN_BUZZER);
  leakSensor = new RealLeakSensor(PIN_LEAK_1, PIN_LEAK_2, PIN_LEAK_3);
  ledGreen = new HardwarePinLed(PIN_LED_GREEN);
  ledYellow = new HardwarePinLed(PIN_LED_YELLOW);
  ledRed = new HardwarePinLed(PIN_LED_RED);
  armButton = new RealButton(systemTime, SPN_BUTTON_LONG_PRESS_TIME, PIN_BUTTON_1);
  maintenanceButton = new RealButton(systemTime, SPN_BUTTON_LONG_PRESS_TIME, PIN_BUTTON_2);
  disarmButton = new RealButton(systemTime, SPN_BUTTON_LONG_PRESS_TIME, PIN_BUTTON_3);
  shutoffValve = new RealShutoffValve();
  waterLevelSensor = new AnalogWaterLevelSensor(
    PIN_WATERLEVEL, WATER_DIST_MIN, WATER_DIST_MAX);
  inputs = new SumpPitInputs(disarmButton, maintenanceButton, armButton);
  rpmSensor1 = new RealRpmSensor(PIN_PUMP_RPM_1, systemTime);
  rpmSensor2 = new RealRpmSensor(PIN_PUMP_RPM_2, systemTime);
  voltageSensor1 = new RealVoltageSensor(PIN_PUMP_VOLTAGE_1);
  voltageSensor2 = new RealVoltageSensor(PIN_PUMP_VOLTAGE_2);
  pump1 = new Pump(systemTime, rpmSensor1, voltageSensor1);
  pump2 = new Pump(systemTime, rpmSensor2, voltageSensor2);
  multiPump = new MultiPump();
  multiPump->addPump(pump1);
  multiPump->addPump(pump2);
  sensor = new SumpPitSensor(waterLevelSensor, 1, leakSensor, 1, multiPump);
  localView = new LocalView(display, ledRed, ledGreen, ledYellow);
  floatSwitch = new RealFloatSwitch(PIN_FLOAT_SWITCH_1, PIN_FLOAT_SWITCH_2);
  node = new SumpPitNode(siren, buzzer, localView, sensor, inputs, shutoffValve, floatSwitch);
  node->setup();

  if (PIN_PUMP_RPM_1 != PIN_NO_PIN) {
    pinMode(PIN_PUMP_RPM_1, INPUT_PULLUP);
    attachInterrupt(PIN_PUMP_RPM_1, triggerRpm1, RISING);
  }
  if (PIN_PUMP_RPM_2 != PIN_NO_PIN) {
    pinMode(PIN_PUMP_RPM_2, INPUT_PULLUP);
    attachInterrupt(PIN_PUMP_RPM_2, triggerRpm2, RISING);
  }

  if (CLOUD_ENABLED) {
    if (SIM_3RD_PARTY) {
      Particle.keepAlive(120);
    }
    bool success = Particle.function("arm", armSystem);
    success = Particle.function("disarm", disarmSystem);
    success = Particle.function("maintenance", startMaintenance);
    success = Particle.function("lcdInit", lcdInit);
    success = Particle.function("update", sendStatusNow);
    success = Particle.function("getLcd", sendScreen);
    success = Particle.function("setDeviceId", setDeviceId);
    Particle.subscribe(PUB_SHUTOFF_STATE, shutoffValveHandler);

    Particle.variable("deviceId", deviceId);
    Particle.variable("mode", node->state.mode);
    Particle.variable("rpm1", node->state.pump1Rpm);
    Particle.variable("rpm2", node->state.pump2Rpm);
    Particle.variable("pump1On", node->state.pump1On);
    Particle.variable("pump2On", node->state.pump2On);
    Particle.variable("status", localView->statusString);

    Particle.variable("p1Uptime", node->state.pump1Uptime);
    Particle.variable("p2Uptime", node->state.pump2Uptime);
    Particle.variable("levelIn", node->state.levelIn);
    Particle.variable("levelP", node->state.levelPercent);
    Particle.variable("leak", node->state.leak);
    Particle.variable("shutoffValve", node->state.shutoffValve);
    Particle.variable("floatSwitch", node->state.floatSwitch);
  }

  Serial.begin(115200);
}

void triggerRpm1() {
  rpmSensor1->trigger();
}

void triggerRpm2() {
  rpmSensor2->trigger();
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
int lcdInit(String extra) {
  display->setup();
  return 0;
}

int lastStatus = -1;
long lastStatusTime = 0L;
char statusString[SPN_STATUS_BITS + 1];
char publishString[256];
long syncPeriod = SYNC_PERIOD_MIN;
bool sendStatus = false;
// loop() runs over and over again, as quickly as it can execute.
void loop() {

  State* state = node->update();

  if (CLOUD_ENABLED && Particle.connected() == 1) {
    if (state->alarmReason != lastStatus) {
      sendStatus = true;
      // status changed
      syncPeriod = SYNC_PERIOD_MIN;
    }
    long now = systemTime->nowMillis();
    if (SYNC_ENABLED && now - lastStatusTime > syncPeriod) {
      sendStatus = true;
      syncPeriod = syncPeriod * 2;
      if (syncPeriod > SYNC_PERIOD_MAX) {
        syncPeriod = SYNC_PERIOD_MAX;
      }
      // only reset update here
      lastStatusTime = systemTime->nowMillis();
    }

    if (sendStatus) {
      sendFullStatus(state);
      sendStatus = false;
    }
  }

  Particle.process();
  /*Serial.print("-----@");
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
  delay(50);*/
}

int setDeviceId(String extra) {
  if (extra.length() > 0) {
    int tmp = extra.toInt();
    if (tmp > 0) {
      deviceId = tmp;
    }
    EEPROM.put(EEPROM_DEVICE_ID, deviceId);
  }
  return deviceId;
}

int sendStatusNow(String extra) {
  sendStatus = true;
  return 0;
}

int sendScreen(String extra) {
  Particle.publish("lcdtext", display->getBank(), PRIVATE);
  return 0;
}

void shutoffValveHandler(const char *event, const char *data)
{
  if ("true" == data) {
    shutoffValve->activate();
  } else if ("false" == data) {
    shutoffValve->deactivate();
  }
}

void sendFullStatus(State* state) {
  statusToString(state->alarmReason, statusString);
  int bars = 0;
  int rssi = 0;
  #if Wiring_WiFi == 1
      rssi = WiFi.RSSI();
  #elif Wiring_Cellular == 1
      CellularSignal sig = Cellular.RSSI();
      rssi = sig.rssi;
  #endif
  if (rssi < 0) {
      if (rssi >= -57) bars = 5;
      else if (rssi > -68) bars = 4;
      else if (rssi > -80) bars = 3;
      else if (rssi > -92) bars = 2;
      else if (rssi > -104) bars = 1;
  }

  sprintf(publishString,
    "{"
    "\"id\":\"%d\","
    "\"technical\":\"%d\","
    "\"critical\":\"%d\","
    "\"shutoffValve\":\"%s\","
    "\"alarm\":\"%s\", \"waterLevel\":\"%d\","
    "\"leakSensor\":\"%s\", \"mode\":\"%d\","
    "\"rpm1\":\"%d\",\"rpm2\":\"%d\","
    "\"uptime\":\"%d\",\"battery\":\"%.2f\","
    "\"rssi\":\"%d\",\"bars\":\"%d\""
    "\"floatSwitch\":\"%s\","
    "}",
    deviceId,
    isTechnical(node->state.alarmReason) ? 1 : 0,
    isCritical(node->state.alarmReason) ? 1 : 0,
    shutoffValve->isActive() ? "1" : "0",
    statusString, waterLevelSensor->measureLevel(),
    leakSensor->isLeaking() ? "1" : "0", node->state.mode,
    rpmSensor1->getRpm(), rpmSensor2->getRpm(),
    systemTime->nowMillis(), fuel.getVCell(),
    rssi, bars,
    floatSwitch->isTriggered() ? "1" : "0"
  );
  Particle.publish("status", publishString, PRIVATE);

  lastStatus = state->alarmReason;
}
