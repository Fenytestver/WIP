#include "impl.ino"

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
// minimum measurable water disatance (inches)
#define WATER_DIST_MIN 4
#define WATER_DIST_MAX 78
#endif

RealSiren siren(PIN_SIREN);
RealBuzzer buzzer(PIN_BUZZER);
RealSystemTime systemTime;
RealLeakSensor leakSensor1(PIN_LEAK_1);
RealLeakSensor leakSensor2(PIN_LEAK_2);
HardwarePinLed ledGreen(PIN_LED_GREEN);
HardwarePinLed ledYellow(PIN_LED_YELLOW);
HardwarePinLed ledRed(PIN_LED_RED);
RealButton button1(&systemTime, PIN_BUTTON_1);
RealButton button2(&systemTime, PIN_BUTTON_2);
RealButton button3(&systemTime, PIN_BUTTON_3);
RealShutoffValve shutoffValve();
UltrasonicWaterLevelSensor waterLevelSensor(
  PIN_WATERLEVEL_TRIG, PIN_WATERLEVEL_ECHO, WATER_DIST_MIN, WATER_DIST_MAX);

class OnArmPress : public OnButtonPressListener {
  public:
    void onPress() {
      Serial.println("Arm pressed");
    }
} armPressListener;

class OnDisarmPress : public OnButtonPressListener {
  public:
    void onPress() {
      Serial.println("Disarm pressed");
    }
} disarmPressListener;

class OnMaintenancePress : public OnButtonPressListener {
  public:
    void onPress() {
      Serial.println("Maintenance pressed");
    }
} mainrenancePressListener;

class OnAnyPress : public OnButtonPressListener {
  public:
    void onPress() {
      buzzer.beep();
    }
} beepPressListener;

/*
 * Project SumpPitSensorProject
 * Description:
 * Author:
 * Date:
 */

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(112500);
  ledGreen.setup();
  ledYellow.setup();
  ledRed.setup();
  button1.setup();
  button2.setup();
  button3.setup();
  buzzer.setup();
  siren.setup();
  // TODO: Move this to node.
  // set up buttons
  button1.setOnButtonLongPressListener(&armPressListener);
  button2.setOnButtonLongPressListener(&mainrenancePressListener);
  button3.setOnButtonLongPressListener(&disarmPressListener);
  button1.setOnButtonPressListener(&beepPressListener);
  button2.setOnButtonPressListener(&beepPressListener);
  button3.setOnButtonPressListener(&beepPressListener);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  button1.update();
  button2.update();
  button3.update();


  // The core of your code will likely live here.
  bool leak = leakSensor1.isLeaking();
  Serial.print("Is leaking = ");
  Serial.println(leak);

  // wait a bit because, why not?
  delay(1000);
}
