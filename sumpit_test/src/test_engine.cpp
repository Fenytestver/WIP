#include "test_engine.h"
#include "led.h"
#include "sumppitsensor.h"
#include "sumppitinputs.h"
#include "pump.h"
#include <iostream>

int main()
{
    std::cout << "Start testing.\n";
    test_engine* suit = new test_engine();
    suit->test_first();
    std::cout << "All tests finished successfully.\n";
}
test_engine::test_engine()
{
  leakSensor = new StubLeakSensor();
  waterLevelSensor = new StubWaterLevelSensor();
  rpmSensor = new StubRpmSensor();
  voltageSensor = new StubVoltageSensor();
  pump = new Pump(rpmSensor, voltageSensor);

  StubButton* disarmButton = new StubButton();
  StubButton* maintenanceButton = new StubButton();
  StubButton* armResetButton = new StubButton();

  // FIXME: only one water level and one leak..
  sensor = new SumpPitSensor(waterLevelSensor, 1, leakSensor, 1, pump);
  inputs = new SumpPitInputs(disarmButton, maintenanceButton, armResetButton);
  siren = new StubSiren();
  display = new StubDisplay();
  inputs = new SumpPitInputs(armButton, maintenanceButton, resetButton);
  node = new SumpPitNode(siren, display, sensor, inputs);
}

test_engine::~test_engine()
{}

void assert(bool b, char* msg) {
  if (!b) {
    std::cerr << "Error: " << msg << "\n";
    exit(1);
  } else {
    std::cout << "Test: \'" << msg << "\' Successful\n";
  }
}

void assert(int actual, int expected, char* msg) {
  if (expected != actual) {
    std::cerr << "Error: " << msg << ", expected:" << expected << ", actual:" << actual << "\n";
    exit(1);
  } else {
    std::cout << "Test: \'" << msg << "\' Successful\n";
  }
}
void assertFalse(bool b, char* msg) {
  assert(!b, msg);
}

void test_engine::test_first()
{
  assert(node->getMode(), SPN_INITIALIZING, "Mode should be initializing");
  node->setup();
  assert(node->getMode(), SPN_ARMED, "Mode is armed after startup");
  assertFalse(siren->isOn(), "Siren must be off after startup");

  // simulate 80% water level.
  std::cout << "Set water level to 80" << std::endl;
  waterLevelSensor->setLevel(80);
  node->update();
  assert(siren->isOn(), "Siren must turn on at this water level.");
  assert(node->getAlarmReason(), SPN_ALARM_HIGH_WATER, "Alarm reason should be high water.");

  // simulate an additional leak as well.
  std::cout << "Set leaking sensor to leaking." << std::endl;
  leakSensor->setLeaking(true);
  node->update();
  assert(siren->isOn(), "Siren must be on with 2 issues detected.");
  assert(node->getAlarmReason(), SPN_ALARM_HIGH_WATER | SPN_ALARM_LEAK, "Alarm reason should be high water + leak.");

  std::cout << "Disarming." << std::endl;
  node->disarm();
  assertFalse(siren->isOn(), "Siren must be off after startup");
  assert(node->getMode(), SPN_DISARMED, "Mode is armed after startup");
}
