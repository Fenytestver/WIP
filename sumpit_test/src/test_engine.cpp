#include "test_engine.h"
#include "led.h"
#include "sumppitsensor.h"
#include "sumppitinputs.h"
#include "pump.h"
#include "debug_util.h"
#include <iostream>
using namespace std;


int XXmain()
{
    std::cout << "\nStart testing pump.\n";
    test_engine* t0 = new test_engine();
    t0->test_pump_turn_on_off();
    std::cout << "\nStart testing first simple run.\n";
    test_engine* t1 = new test_engine();
    t1->test_first();
    std::cout << "\nStart testing pump failure.\n\n";
    test_engine* t2 = new test_engine();
    t2->test_pump_failure_at_start();
    std::cout << "\nAll tests finished successfully.\n";
}
test_engine::test_engine()
{
  leakSensor = new StubLeakSensor();
  waterLevelSensor = new StubWaterLevelSensor();
  rpmSensor = new StubRpmSensor();
  voltageSensor = new StubVoltageSensor();
  systemTime = new StubSystemTime();
  // time should not be 0, that would be a problem.
  systemTime->setTime(1L);

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

void test_engine::test_pump_turn_on_off()
{
  assert(pump->getUptime(), 0L, "Pump uptime should be 0 at start.");
  long now = systemTime->nowMillis();
  pump->turnOn();
  assert(pump->isTurnedOn(), "Pump turned on.");
  systemTime->addTime(1L);
  assert(systemTime->nowMillis(), now + 1L, "1ms passed since 'now'.");
  assert(pump->getUptime(), 1L, "1ms since we started the pump.");
  // now turn off the pump
  pump->turnOff();
  assert(pump->getUptime(), 0L, "Pump turned off, no uptime (0).");
  assertFalse(pump->isTurnedOn(), "Pump turned off.");
}


void test_engine::test_first()
{
  assert(node->getMode(), SPN_INITIALIZING, "Mode should be initializing");
  node->setup();
  assert(node->getMode(), SPN_ARMED, "Mode is armed after startup");
  assertFalse(siren->isOn(), "Siren must be off after startup");

  // simulate 80% water level.
  std::cout << "Set water level to SPN_WATER_HIGH" << std::endl;
  waterLevelSensor->setLevel(SPN_WATER_HIGH);
  node->update();
  assert(pump->isTurnedOn(), "Pump must turn on at this water level.");

  // simulate an additional leak as well.
  leakSensor->setLeaking(true);
  node->update();
  assert(siren->isOn(), "Siren must be on with 2 issues detected.");
  assert(node->getAlarmReason(), SPN_ALARM_WATER_CRITICAL | SPN_ALARM_LEAK, "Alarm reason should be high water + leak.");

  node->disarm();
  assertFalse(siren->isOn(), "Siren must be off after startup");
  assert(node->getMode(), SPN_DISARMED, "Mode is armed after startup");
}

void test_engine::test_pump_failure_at_start() {
  systemTime->setTime(1);
  node->setup();
  waterLevelSensor->setLevel(SPN_WATER_CRITICAL);
  node->update();
  assert(pump->isTurnedOn(), "Pump must be running");
  assert(siren->isOn(), "Siren must turn on at this water level.");
  // set very low voltage, rpm
  voltageSensor->setVoltage(0.1);
  rpmSensor->setRpm(10);
  cout << "pumUp: " << pump->getUptime() << ", time" << systemTime->nowMillis() << endl;
  node->update();
  // timeout not reached, should not be a problem.
  assert(node->getAlarmReason(), SPN_ALARM_WATER_CRITICAL, "High water alarm only, motor did not have time to spin up.");

  cout << pump->getUptime() << endl;
  //systemTime->addTime(1L);
  cout << pump->getUptime() << endl;
  assert(pump->getUptime() > SPS_PUMP_SPINUP_TIME, "SPS_PUMP_SPINUP_TIME time passed");
  // We may BTW assume if pump has no voltage will not have rpm.
  assert(node->getAlarmReason(), SPN_ALARM_WATER_CRITICAL | SPN_ALARM_PUMP_RPM_CRITICAL | SPN_ALARM_PUMP_VOLTAGE_CRITICAL,
         "Motor failed to start with multiple problems.");
}
