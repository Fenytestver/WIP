#include "test_pump_failure_functional.h"

test_pump_failure_functional::test_pump_failure_functional()
  : TestBase("Pump fails to start scenario")
{
  //ctor
}

test_pump_failure_functional::~test_pump_failure_functional()
{
  //dtor
}

void test_pump_failure_functional::test() {
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
