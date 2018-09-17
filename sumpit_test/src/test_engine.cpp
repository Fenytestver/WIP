#include "test_engine.h"
#include "led.h"
#include "sumppitsensor.h"
#include "sumppitinputs.h"
#include "pump.h"
#include "debug_util.h"
#include <iostream>
using namespace std;

void assert(bool b, string msg) {
  if (!b) {
    std::cerr << "Error: " << msg << "\n";
    exit(1);
  } else {
    std::cout << "Test: \'" << msg << "\' Successful\n";
  }
}

void assert(int actual, int expected, string msg) {
  if (expected != actual) {
    std::cerr << "Error: " << msg << ", expected:" << expected << ", actual:" << actual << "\n";
    exit(1);
  } else {
    std::cout << "Test: \'" << msg << "\' Successful\n";
  }
}
void assertFalse(bool b, string msg) {
  assert(!b, msg);
}

int main()
{
    std::cout << "Start testing.\n";
    test_engine* t1 = new test_engine();
    t1->test_first();
    std::cout << "Test finished.\n";
    test_engine* t2 = new test_engine();
    t2->test_pump_failure_at_start();
    std::cout << "Test pump failure finished.\n";

    std::cout << "All tests finished successfully.\n";
}
test_engine::test_engine()
{
  leakSensor = new StubLeakSensor();
  waterLevelSensor = new StubWaterLevelSensor();
  rpmSensor = new StubRpmSensor();
  voltageSensor = new StubVoltageSensor();
  systemTime = new StubSystemTime();
  pump = new Pump(systemTime, rpmSensor, voltageSensor);

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
  leakSensor->setLeaking(true);
  node->update();
  assert(siren->isOn(), "Siren must be on with 2 issues detected.");
  assert(node->getAlarmReason(), SPN_ALARM_HIGH_WATER | SPN_ALARM_LEAK, "Alarm reason should be high water + leak.");

  node->disarm();
  assertFalse(siren->isOn(), "Siren must be off after startup");
  assert(node->getMode(), SPN_DISARMED, "Mode is armed after startup");
}

void test_engine::test_pump_failure_at_start() {
  systemTime->setTime(0);
  cout << "pumUp: " << pump->getPumpUptime() << ", time" << systemTime->nowMillis() << endl;
  node->setup();
  waterLevelSensor->setLevel(90);
  node->update();
  cout << "pumUp: " << pump->getPumpUptime() << ", time" << systemTime->nowMillis() << endl;
  assert(siren->isOn(), "Siren must turn on at this water level.");
  assert(pump->isTurnedOn(), "Pump must be running");
  // set very low voltage, rpm
  voltageSensor->setVoltage(0.1);
  rpmSensor->setRpm(10);
  cout << "pumUp: " << pump->getPumpUptime() << ", time" << systemTime->nowMillis() << endl;
  node->update();
  // timeout not reached, should not be a problem.
  assert(node->getAlarmReason(), SPN_ALARM_HIGH_WATER, "High water alarm only, motor did not have time to spin up.");

  cout << pump->getPumpUptime() << endl;
  systemTime->addTime(SPS_PUMP_SPINUP_TIME + 1);
  cout << pump->getPumpUptime() << endl;
  assert(pump->getPumpUptime() > SPS_PUMP_SPINUP_TIME, "SPS_PUMP_SPINUP_TIME time passed");
  // We may BTW assume if pump has no voltage will not have rpm.
  assert(node->getAlarmReason(), SPN_ALARM_HIGH_WATER | SPN_ALARM_PUMP_RPM_FAILURE | SPN_ALARM_PUMP_VOLTAGE_FAILURE,
         "Motor failed to start with multiple problems.");
}
