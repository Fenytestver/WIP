#include "Particle.h"
// STARTUP(cellular_credentials_set("pwg", "", "", NULL));

#include "spn_config.h"
#include "button.h"
#include "systemtime.h"
#include "buzzer.h"
#include "impl.ino"
//#include <ArduinoJson.h>

#include "RdJson.h"

/*
 * Project SumpPitActuatorProject
 * Description: The Actuator for the water shutoff valve.
 * Author:
 * Date:
 */

#define EEPROM_CLUSTER_ID (sizeof(int) * 11)
#define BUTTON_LONG_PRESS_TIME 5000
#define ACTUATOR_CYCLE_TIME 35000
#define BEEP_TIME 50
#define SHUTOFF_BEEP_TIME 300
#define SYSTEM_STATUS_TOPIC "spnStatus"
#define SHUTOFF_VALVE_TOPIC "shutoffValve"
#define SHUTOFF_VALVE_EXPECTED_TOPIC "shutoffExpected"
#define SHUTOFF_ANOMALY_TOPIC "shutoffAnomaly"
#define POWER_ALARM_TOPIC "shutoffPwrCrit"
#define SPN_MODE_UNKNOWN -1

#define PIN_SHUTOFF_ON D2
#define PIN_SHUTOFF_OFF C3
#define PIN_BUTTON_OPEN C2
#define PIN_BUTTON_CLOSE C1
#define PIN_BUZZER D7
#define PIN_LED_GREEN D5
#define PIN_LED_YELLOW D4
#define PIN_LED_RED D3
#define PIN_LED_OPEN A0
#define PIN_LED_CLOSED A1
#define PIN_DETECT_OPEN B4
#define PIN_DETECT_CLOSED B5
#ifdef B3
// ELECTRON ONLY
#define PIN_ACTUATOR_POWER B3
#else
#define PIN_ACTUATOR_POWER D0
#endif
SystemTime* systemTime;
RealButton* openButton;
RealButton* closeButton;
RealButton* openDetector;
RealButton* closeDetector;
RealButton* powerDetector;
FunctionPressListener* openPressListener;
FunctionPressListener* closePressListener;
RealBuzzer* buzzer;
HardwarePinLed* ledGreen;
HardwarePinLed* ledYellow;
HardwarePinLed* ledRed;
HardwarePinLed* openLed;
HardwarePinLed* closedLed;
RealBuzzer* openRelay;
RealBuzzer* closeRelay;

typedef struct DeviceStatus {
  int id;
  bool technical;
  bool critical;
  int mode;
  long alarmReason;
};

#define MAX_DEVICE_COUNT 5
DeviceStatus statusArray[MAX_DEVICE_COUNT];
int numDevices = 0;

//long alarmReason = SPN_ALARM_NO_ALARM;
char* dataCpy = new char[1024];
bool stateUnknown = true;
unsigned long nothingDetectedSince = 0L;
Timer keepAliveTimer(300000, sendKeepAlivePacket);

class OnAnyPress : public OnButtonPressListener {
      public:
        OnAnyPress(Buzzer* _buzzer) {buzzer=_buzzer;}
        void onPress() {
          buzzer->beep();
        }
      private:
        Buzzer* buzzer;
    } *beepPressListener;

bool shutoffEnabled = false;
bool shutoffExpected = false;
unsigned long expectedAt = 0L;
bool powerAlarm = true;
bool shutoffAnomaly = false;
char message[256];
bool sendKeepAlive = false;
SerialDebugOutput debugOutput(115200, ALL_LEVEL);
int clusterId = 0;

bool publish(char* topic, char* value) {
  char topicWithCluster[128];
  sprintf(topicWithCluster, "c%d/%s", clusterId, topic);
  return Particle.publish(String(topicWithCluster), String(value), PRIVATE);
}

bool subscribe(char* topic, void (*handler)(const char*, const char*)) {
  char topicWithCluster[128];
  sprintf(topicWithCluster, "c%d/%s", clusterId, topic);
  return Particle.subscribe(String(topicWithCluster), handler, MY_DEVICES);
}

void setup() {
  Serial.begin(115200);
  INFO("Starting");
  clusterId = getEepromInt(EEPROM_CLUSTER_ID, 0);
  systemTime = new RealSystemTime();
  ledGreen = new HardwarePinLed(PIN_LED_GREEN);
  ledYellow = new HardwarePinLed(PIN_LED_YELLOW);
  ledRed = new HardwarePinLed(PIN_LED_RED);
  openLed = new HardwarePinLed(PIN_LED_OPEN);
  closedLed = new HardwarePinLed(PIN_LED_CLOSED);
  buzzer = new RealBuzzer(PIN_BUZZER, BEEP_TIME);
  openButton = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_BUTTON_OPEN);
  closeButton = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_BUTTON_CLOSE);
  powerDetector = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_ACTUATOR_POWER);
  openPressListener = new FunctionPressListener(openPublish);
  closePressListener = new FunctionPressListener(closePublish);
  beepPressListener = new OnAnyPress(buzzer);
  powerDetector->setOnButtonPressListener(new FunctionPressListener(powerChange));
  powerDetector->setOnButtonLongPressListener(new FunctionPressListener(powerChange));
  openButton->setOnButtonPressListener(beepPressListener);
  openButton->setOnButtonLongPressListener(openPressListener);
  closeButton->setOnButtonPressListener(beepPressListener);
  closeButton->setOnButtonLongPressListener(closePressListener);
  openDetector = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_DETECT_OPEN);
  closeDetector = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_DETECT_CLOSED);
  openDetector->setOnButtonPressListener(new FunctionPressListener(openDetected));
  closeDetector->setOnButtonPressListener(new FunctionPressListener(closeDetected));
  openDetector->setOnButtonLongPressListener(new OnButtonPressListener());
  closeDetector->setOnButtonLongPressListener(new OnButtonPressListener());
  openRelay = new RealBuzzer(PIN_SHUTOFF_OFF, SHUTOFF_BEEP_TIME);
  closeRelay = new RealBuzzer(PIN_SHUTOFF_ON, SHUTOFF_BEEP_TIME);
  ledGreen->setup();
  ledRed->setup();
  ledYellow->setup();
  powerDetector->setup();
  buzzer->setup();
  openLed->setup();
  closedLed->setup();
  openButton->setup();
  closeButton->setup();
  openRelay->setup();
  closeRelay->setup();
  subscribe(SHUTOFF_VALVE_EXPECTED_TOPIC, shutoffValveHandler);
  Particle.function("closeWater", closeWater);
  Particle.function("openWater", openWater);
  Particle.function("reboot", reboot);
  Particle.function("setClusterId", setClusterId);
  Particle.variable("clusterId", clusterId);
  subscribe(SYSTEM_STATUS_TOPIC, statusHandler);
  publish(SHUTOFF_ANOMALY_TOPIC, "false");
  // request immediate update.
  publish("spnPing", "anyonethere");
  keepAliveTimer.start();
}

long count = 0;

void loop() {
  unsigned long now = systemTime->nowMillis();
  long nowbit = now / 200;
  bool technical = isTechnical();
  bool critical = isCritical();
  long alarmReason = getAlarmReason();
  int armed = 0;
  int disarmed = 0;
  int maintenance = 0;
  if ((count++) % 30 == 0) {
    INFO("### loop: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
        shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
  }
  for (int i = 0; i < numDevices; ++i) {
    switch (statusArray[i].mode) {
      case SPN_ARMED:
        armed++;
        break;
      case SPN_DISARMED:
        disarmed++;
        break;
      case SPN_MAINTENANCE:
      default:
        maintenance++;
        break;
    }
  }

  // force critial in case of an anomaly
  if (shutoffAnomaly) {
    critical = true;
  }

  if (numDevices > 0) {
    ledGreen->setState(!technical && !critical && armed > 0 && maintenance == 0);
    ledYellow->setState(maintenance > 0
        || (armed == 0)
        || (technical && ((nowbit % 2) == 0)));
    ledRed->setState(critical && maintenance == 0);
  } else {
    ledRed->setState(false);
    ledGreen->setState((nowbit % 2) == 0);
    ledYellow->setState((nowbit % 2) == 1);
  }
  if (shutoffExpected && !powerDetector->isPressed()) {
    ledRed->setState(true);
    if (!powerAlarm) {
      INFO("### power alarm: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
          shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
      powerAlarm = true;
      publish(POWER_ALARM_TOPIC, "true");
    }
  } else if (powerAlarm) {
    powerAlarm = false;
    INFO("### off power alarm: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
        shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
    publish(POWER_ALARM_TOPIC, "false");
  }
  if (stateUnknown || (shutoffEnabled != shutoffExpected)) {
    if (!shutoffAnomaly && !stateUnknown && (expectedAt > 0) && (now - expectedAt > ACTUATOR_CYCLE_TIME)) {
      INFO("### Enabling anomaly: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
        shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
      shutoffAnomaly = true;
      Serial.println("ANOMALY!!");
      publish(SHUTOFF_ANOMALY_TOPIC, "true");
    }
    INFO("### Blink leds: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
      shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
    openLed->setState((nowbit % 2) == 0);
    closedLed->setState((nowbit % 2) == 1);
  } else {
    if (shutoffAnomaly && shutoffEnabled == shutoffExpected) {
      shutoffAnomaly = false;
      INFO("### off anomaly: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
          shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
      publish(SHUTOFF_ANOMALY_TOPIC, "false");
    }
    openLed->setState(!shutoffEnabled);
    closedLed->setState(shutoffEnabled);
  }
  openButton->update();
  closeButton->update();
  openDetector->update();
  closeDetector->update();
  powerDetector->update();
  if (!openDetector->isPressed() && !closeDetector->isPressed()) {
    INFO("### nothing detected: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
        shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
    nothingDetected();
  } else if (stateUnknown) {
    if (openDetector->isPressed()) {
      INFO("### opened-signal detected in unknown state: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
          shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
      openDetected();
    } else if (closeDetector->isPressed()) {
      INFO("### closed-signal detected in unknown state: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
          shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
      closeDetected();
    }
  }
  if (sendKeepAlive) {
    INFO("### Sending keep alive packet: shutoffExpected=%d, shutoffEnabled=%d, shutoffAnomaly=%d, expectedAt=%u, now=%u",
        shutoffExpected, shutoffEnabled, shutoffAnomaly, expectedAt, now);
    sendKeepAlive = false;
    int uptime = systemTime->nowMillis() / 1000;
    sprintf(message, "{\"uptime\": \"%d\", \"pwr\": \"%s\"}",
        uptime,
        powerDetector->isPressed() ? "1" : "0");
    publish("spnActuator/status", message);
  }
  delay(100);
}

bool open(bool publishIt) {
  bool result = false;
  // shutoffExpected mean we wanted the valve to close water.
  if (shutoffExpected || stateUnknown) {
    buzzer->beep();
    openRelay->beep();
    // stateUnknown = true;
    INFO("### opening valve by request (publish=%d).",publishIt);
    if (publishIt) {
      publish(SHUTOFF_VALVE_EXPECTED_TOPIC, "false");
    }
    result = true;
  } else {
    INFO("### NOT opening valve by request, valve already open (publish=%d).",publishIt);
  }
  if (shutoffExpected) {
    expectedAt = systemTime->nowMillis();
    shutoffExpected = false;
  }
  return result;
}

bool close(bool publishIt) {
  bool result = false;
  if (!shutoffExpected || stateUnknown) {
    buzzer->beep();
    closeRelay->beep();
    stateUnknown = true;
    INFO("### closing valve by request (publish=%d).",publishIt);
    if (publishIt) {
      publish(SHUTOFF_VALVE_EXPECTED_TOPIC, "true");
    } else {
      INFO("### not closing valve by request, valve already closed (publish=%d).",publishIt);
      Serial.println("Not closing, already closed");
    }
    result = true;
  }
  if (!shutoffExpected) {
    expectedAt = systemTime->nowMillis();
    shutoffExpected = true;
  }
  delay(100);
  return result;
}
void closeDetected() {
  stateUnknown = false;
  if (!shutoffEnabled) {
    INFO("### close detected. shutoffExpected=%d", shutoffExpected);
  }
  shutoffEnabled = true;
  nothingDetectedSince = 0L;
  if (shutoffExpected) {
    publish(SHUTOFF_VALVE_TOPIC, "true");
  }
}
void openDetected() {
  if (!shutoffEnabled) {
    INFO("### open detected. shutoffExpected=%d", shutoffExpected);
  }
  stateUnknown = false;
  shutoffEnabled = false;
  nothingDetectedSince = 0L;
  if (!shutoffExpected) {
    publish(SHUTOFF_VALVE_TOPIC, "false");
  }
}
void powerChange() {
}
void nothingDetected() {
  unsigned long now = systemTime->nowMillis();
  if (nothingDetectedSince == 0L) {
    INFO("### nothing detected. now=%u", now);
    nothingDetectedSince = now;
    buzzer->beep();
  } else if (now - nothingDetectedSince > ACTUATOR_CYCLE_TIME) {
    stateUnknown = true;
  }
}

void openPublish() {
  INFO("### Open+publish from button press");
  open(true);
}
void closePublish() {
  INFO("### Close+publish from button press");
  close(true);
}

void shutoffValveHandler(const char *event, const char *data)
{
  INFO("[handler]shutoff changed to: %s", data);
  if (strcmp("true", data) == 0) {
    INFO("Closing from handler");
    close(false);
  } else if (strcmp("false", data) == 0) {
    INFO("Opening from handler");
    open(false);
  }
}

int openWater(String extra) {
  INFO("open remote request. extra: %s", extra);
  return open(true) ? 1 : 0;
}
int closeWater(String extra) {
  INFO("close remote request. extra: %s", extra);
  return close(true) ? 1 : 0;
}

int stringToInt(String extra, int defaultInt) {
  int result = defaultInt;
  if (extra.length() > 0) {
    int tmp = extra.toInt();
    if (tmp > 0) {
      result = tmp;
    }
  }
  return result;
}

int reboot(String extra) {
  System.reset();
  return 1;
}


DeviceStatus* getStatusById(int id) {
  for (int i=0; i<numDevices; ++i) {
    if (statusArray[i].id = id) {
      return &statusArray[i];
    }
  }
  if (numDevices < MAX_DEVICE_COUNT) {
    DeviceStatus status;
    status.id = id;
    status.technical = false;
    status.critical = false;
    status.mode = SPN_MODE_UNKNOWN;
    status.alarmReason = SPN_ALARM_NO_ALARM;
    statusArray[numDevices] = status;
    numDevices++;
    return &statusArray[numDevices];
  }
  // FIXME: what if there's another device?
  return nullptr;
}
bool isTechnical() {
  for (int i = 0; i < numDevices; ++i) {
    if (statusArray[i].mode != SPN_MAINTENANCE && statusArray[i].technical) {
      return true;
    }
  }
  return false;
}
bool isCritical() {
  for (int i = 0; i < numDevices; ++i) {
    if (statusArray[i].mode != SPN_MAINTENANCE && statusArray[i].critical) {
      return true;
    }
  }
  return false;
}
long getAlarmReason() {
  long alarmReason = SPN_ALARM_NO_ALARM;
  for (int i = 0; i < numDevices; ++i) {
    alarmReason |= statusArray[i].alarmReason;
  }
  return alarmReason;
}

void statusHandler(const char* event, const char* data) {

  Serial.print("event: ");
  Serial.println(event);
  Serial.print("update: ");
  Serial.print(data);
  Serial.println();
  strcpy(dataCpy, data);
  Serial.print(dataCpy);
  Serial.println();

  Serial.print("Mode ");
  bool isValid = false;
  jsmnrtype_t objType = JSMNR_UNDEFINED;
  int objSize = 0;

  String idStr = RdJson::getString("id", "", isValid, objType, objSize, dataCpy);
  int id = stringToInt(idStr, -1);
  DeviceStatus* thisStatus = getStatusById(id);
  if (thisStatus == nullptr) {
    Serial.print("unaccepted id ");
    Serial.println(id);
    return;
  }

  String mStr = RdJson::getString("mode", "", isValid, objType, objSize, dataCpy);

  thisStatus->mode = stringToInt(mStr, SPN_MODE_UNKNOWN);
  thisStatus->technical = RdJson::getString("technical", "", isValid, objType, objSize, dataCpy) == "1";
  thisStatus->critical = RdJson::getString("critical", "", isValid, objType, objSize, dataCpy) == "1";
  INFO("Receive status: %s", data);
  Serial.println(mStr);
  Serial.print("technical");
  Serial.println(thisStatus->technical);
  Serial.print("critical");
  Serial.println(thisStatus->critical);
}

void sendKeepAlivePacket() {
  sendKeepAlive = true;
}

int getEepromInt(int address, int def) {
  int tmp = -1;
  EEPROM.get(address, tmp);
  if (tmp > -1) {
    return tmp;
  }
  return def;
}

int setClusterId(String extra) {
  int extraInt = stringToInt(extra, -1);
  if (extraInt > 0) {
    clusterId = extraInt;
    EEPROM.put(EEPROM_CLUSTER_ID, clusterId);
  }
  return clusterId;
}
