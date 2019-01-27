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

SystemTime* systemTime;
RealButton* openButton;
RealButton* closeButton;
FunctionPressListener* openPressListener;
FunctionPressListener* closePressListener;
RealBuzzer* buzzer;
HardwarePinLed* ledGreen;
HardwarePinLed* ledYellow;
HardwarePinLed* ledRed;
RealBuzzer* openRelay;
RealBuzzer* closeRelay;
//long alarmReason = SPN_ALARM_NO_ALARM;
bool technical = false;
bool critical = false;
int mode = SPN_MODE_UNKNOWN;
char* dataCpy = new char[1024];
bool stateUnknown = true;

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
  openRelay = new RealBuzzer(PIN_SHUTOFF_OFF, SHUTOFF_BEEP_TIME);
  closeRelay = new RealBuzzer(PIN_SHUTOFF_ON, SHUTOFF_BEEP_TIME);
  ledGreen->setup();
  ledRed->setup();
  ledYellow->setup();
  buzzer->setup();
  openButton->setup();
  closeButton->setup();
  openRelay->setup();
  closeRelay->setup();
  Particle.subscribe(SHUTOFF_VALVE_TOPIC, shutoffValveHandler);
  Particle.function("closeWater", closeWater);
  Particle.function("openWater", openWater);
  Particle.subscribe(SYSTEM_STATUS_TOPIC, statusHandler);
}

void loop() {
  long now = systemTime->nowMillis();
  long nowSec = now / 1000;
  switch (mode) {
    case SPN_MODE_UNKNOWN:
      ledRed->setState((nowSec % 3) == 0);
      ledGreen->setState((nowSec % 3) == 1);
      ledYellow->setState((nowSec % 3) == 2);
      break;
    case SPN_ARMED:
      ledRed->setState(critical);
      ledGreen->setState(true);
      ledYellow->setState(technical);
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
  openButton->update();
  closeButton->update();
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

  String mStr = RdJson::getString("mode", "", isValid, objType, objSize, dataCpy);
  Serial.println(mStr);
  mode = stringToInt(mStr, SPN_MODE_UNKNOWN);

  technical = RdJson::getString("technical", "", isValid, objType, objSize, dataCpy) == "1";
  critical = RdJson::getString("critical", "", isValid, objType, objSize, dataCpy) == "1";
  Serial.print("technical");
  Serial.println(technical);
  Serial.print("critical");
  Serial.println(critical);
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
