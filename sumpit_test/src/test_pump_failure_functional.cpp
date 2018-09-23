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
  waterLevelSensor1->setLevel(SPN_WATER_CRITICAL);
  node->update();
  assert(pump->isTurnedOn(), "Pump must be running");
  assert(siren->isOn(), "Siren must turn on at this water level.");
  // set very low voltage, rpm
  voltageSensor->setVoltage(0.1);
  // set a just critical rpm.
  rpmSensor->setRpm(SPN_PUMP_STD_RPM - SPN_PUMP_RPM_DEVI_CRITICAL - 1);
  node->update();
  // timeout not reached, should not be a problem.
  assert(node->getAlarmReason() & SPN_ALARM_WATER_CRITICAL,
         "High water alarm must be on at critical water level.");
  assertFalse((node->getAlarmReason() & (SPN_ALARM_PUMP_RPM_CRITICAL
                                   | SPN_ALARM_PUMP_VOLTAGE_CRITICAL)) != 0,
         "Motor just starting up, shouldn't be a problem (yet).");
  // let it run for a while
  systemTime->addTime(SPS_PUMP_SPINUP_TIME * 2);
  node->update();
  // TODO: all pumps to run at the same time
  assert(pump->getUptime() >= SPS_PUMP_SPINUP_TIME, "SPS_PUMP_SPINUP_TIME time passed");
  // We may BTW assume if pump has no voltage will not have rpm.
  assert((node->getAlarmReason() & (SPN_ALARM_WATER_CRITICAL
                                   | SPN_ALARM_PUMP_RPM_CRITICAL
                                   | SPN_ALARM_PUMP_VOLTAGE_CRITICAL)) != 0,
         "Motor failed to start with multiple problems.");
}
