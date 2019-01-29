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

#define BUTTON_LONG_PRESS_TIME 3000
#define BEEP_TIME 50
#define SHUTOFF_BEEP_TIME 300
#define SYSTEM_STATUS_TOPIC "spnStatus"
#define SHUTOFF_VALVE_TOPIC "shutoffValve"
#define SPN_MODE_UNKNOWN -1

#define PIN_SHUTOFF_ON D1
#define PIN_SHUTOFF_OFF D2
#define PIN_BUTTON_OPEN A4
#define PIN_BUTTON_CLOSE A3
#define PIN_BUZZER D3
#define PIN_LED_GREEN D5
#define PIN_LED_YELLOW D4
#define PIN_LED_RED D6
#define PIN_LED_OPEN A1
#define PIN_LED_CLOSED A2
#define PIN_DETECT_OPEN A5
#define PIN_DETECT_CLOSED A0

SystemTime* systemTime;
RealButton* openButton;
RealButton* closeButton;
RealButton* openDetector;
RealButton* closeDetector;
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
 //SerialDebugOutput debugOutput(115200);

void setup() {
  Serial.begin(115200);
  systemTime = new RealSystemTime();
  ledGreen = new HardwarePinLed(PIN_LED_GREEN);
  ledYellow = new HardwarePinLed(PIN_LED_YELLOW);
  ledRed = new HardwarePinLed(PIN_LED_RED);
  openLed = new HardwarePinLed(PIN_LED_OPEN);
  closedLed = new HardwarePinLed(PIN_LED_CLOSED);
  buzzer = new RealBuzzer(PIN_BUZZER, BEEP_TIME);
  openButton = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_BUTTON_OPEN);
  closeButton = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_BUTTON_CLOSE);
  openPressListener = new FunctionPressListener(openPublish);
  closePressListener = new FunctionPressListener(closePublish);
  beepPressListener = new OnAnyPress(buzzer);
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
  buzzer->setup();
  openLed->setup();
  closedLed->setup();
  openButton->setup();
  closeButton->setup();
  openRelay->setup();
  closeRelay->setup();
  Particle.subscribe(SHUTOFF_VALVE_TOPIC, shutoffValveHandler);
  Particle.function("closeWater", closeWater);
  Particle.function("openWater", openWater);
  Particle.function("reboot", reboot);
  Particle.subscribe(SYSTEM_STATUS_TOPIC, statusHandler);
  // request immediate update.
  Particle.publish("spnPing", "anyonethere");
}

void loop() {
  long now = systemTime->nowMillis();
  long nowbit = now / 200;
  bool technical = isTechnical();
  bool critical = isCritical();
  int mode = getMode();
  long alarmReason = getAlarmReason();
  switch (mode) {
    case SPN_MODE_UNKNOWN:
      ledRed->setState((nowbit % 3) == 0);
      ledGreen->setState((nowbit % 3) == 1);
      ledYellow->setState((nowbit % 3) == 2);
      break;
    case SPN_ARMED:
      ledRed->setState(critical);
      ledGreen->setState(true);
      ledYellow->setState(technical && ((nowbit % 2) == 0));
      break;
    case SPN_DISARMED:
      ledRed->setState(true);
      ledGreen->setState(false);
      ledYellow->setState(true);
      break;
    case SPN_MAINTENANCE:
      ledRed->setState(false);
      ledGreen->setState(false);
      ledYellow->setState(true);
      break;
  }
  if (stateUnknown) {
    openLed->setState((nowbit % 2) == 0);
    closedLed->setState((nowbit % 2) == 1);
  } else {
    openLed->setState(!shutoffEnabled);
    closedLed->setState(shutoffEnabled);
  }
  openButton->update();
  closeButton->update();
  openDetector->update();
  closeDetector->update();
  if (!openDetector->isPressed() && !closeDetector->isPressed()) {
    nothingDetected();
  }
  delay(50);
}

bool open(bool publish) {
  bool result = false;
  if (shutoffEnabled || stateUnknown) {
    buzzer->beep();
    openRelay->beep();
    if (publish) {
      shutoffEnabled = false;
      stateUnknown = false;
      Particle.publish(SHUTOFF_VALVE_TOPIC, "false", PUBLIC);
    }
    result = true;
  } else {
    Serial.println("Not opening, already open");
  }
  shutoffEnabled = false;
  stateUnknown = false;
  return result;
}

bool close(bool publish) {
  bool result = false;
  if (!shutoffEnabled || stateUnknown) {
    buzzer->beep();
    closeRelay->beep();
    if (publish) {
      shutoffEnabled = true;
      stateUnknown = false;
      Particle.publish(SHUTOFF_VALVE_TOPIC, "true", PUBLIC);
    } else {
      Serial.println("Not closing, already closed");
    }
    result = true;
  }
  shutoffEnabled = true;
  stateUnknown = false;
  return result;
}
void closeDetected() {
  stateUnknown = false;
  shutoffEnabled = true;
  nothingDetectedSince = 0L;
}
void openDetected() {
  stateUnknown = false;
  shutoffEnabled = false;
  nothingDetectedSince = 0L;
}
void nothingDetected() {
  unsigned long now = systemTime->nowMillis();
  if (nothingDetectedSince == 0L) {
    nothingDetectedSince = now;
    buzzer->beep();
  } else if (now - nothingDetectedSince > 5000) {
    stateUnknown = true;
  }
}

void openPublish() {
  open(true);
}
void closePublish() {
  close(true);
}

void shutoffValveHandler(const char *event, const char *data)
{
  Serial.print("shutoff changed to: ");
  Serial.println(data);
  if (strcmp("true", data) == 0) {
    Serial.println("Closing");
    close(false);
  } else if (strcmp("false", data) == 0) {
    Serial.println("Opening");
    open(false);
  }
}

int openWater(String extra) {
  return open(true) ? 1 : 0;
}
int closeWater(String extra) {
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
    if (statusArray[i].technical) {
      return true;
    }
  }
  return false;
}
bool isCritical() {
  for (int i = 0; i < numDevices; ++i) {
    if (statusArray[i].critical) {
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

int getMode() {
  int mode = SPN_MODE_UNKNOWN;
  for (int i = 0; i < numDevices; ++i) {
    if (statusArray[i].mode > mode) {
      mode = statusArray[i].mode;
    }
  }
  return mode;
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

  Serial.println(mStr);
  Serial.print("technical");
  Serial.println(thisStatus->technical);
  Serial.print("critical");
  Serial.println(thisStatus->critical);
}
