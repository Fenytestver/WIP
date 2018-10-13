#include "test_pump_failure_functional.h"

test_pump_failure_functional::test_pump_failure_functional()
  : FunctionalTestBase("Pump fails to start scenario")
{
  //ctor
}

test_pump_failure_functional::~test_pump_failure_functional()
{
  //dtor
}

void test_pump_failure_functional::test() {
  node->setup();
  waterLevelSensor->setLevel(SPN_WATER_CRITICAL);
  voltageSensor1->setVoltage(12.0);
  voltageSensor2->setVoltage(12.0);
  node->update();
  assert(subPump1->isTurnedOn(), "SubPump1 must be running");
  assert(subPump2->isTurnedOn(), "SubPump2 must be running");
  assert(siren->isOn(), "Siren must turn on at this water level.");
  // set a just critical rpm.
  rpmSensor->setRpm(SPN_PUMP_STD_RPM - SPN_PUMP_RPM_DEVI_CRITICAL);
  node->update();

  assert(shutoffValve->isActive(), "Shut off valve should be active at critical water level.");
  // timeout not reached, should not be a problem.
  assert((node->getAlarmReason() & SPN_ALARM_WATER_CRITICAL) != 0,
         "High water alarm must be on at critical water level.");
  assertFalse((node->getAlarmReason() & (SPN_ALARM_PUMP_RPM_CRITICAL
                                   | SPN_ALARM_PUMP_VOLTAGE_CRITICAL)) != 0,
         "Motor just starting up, shouldn't be a problem (yet).");
  // let it run for a while
  systemTime->addTime(SPN_PUMP_SPINUP_TIME * 2);
  node->update();
  assert(subPump1->getUptime() >= SPN_PUMP_SPINUP_TIME, "P1 SPN_PUMP_SPINUP_TIME time passed");
  assert(subPump2->getUptime() >= SPN_PUMP_SPINUP_TIME, "P2 SPN_PUMP_SPINUP_TIME time passed");
  // We may BTW assume if pump has no voltage will not have rpm.
  assertAllFlags(node->getAlarmReason(), SPN_ALARM_WATER_CRITICAL
                                   | SPN_ALARM_PUMP_RPM_CRITICAL,
         "Motor failed to start with multiple problems.");

  waterLevelSensor->setLevel(SPN_WATER_CRITICAL - 1);
  node->update();
  assert((node->getAlarmReason() & SPN_ALARM_WATER_CRITICAL) != 0, "WaterLevel did not drop to forget critical.");

  waterLevelSensor->setLevel(SPN_WATER_CRITICAL - SPN_WATER_VARIANCE - 10);
  node->update();
  assert((node->getAlarmReason() & SPN_ALARM_WATER_CRITICAL), 0, "WaterLevel below critical-variance.");

  rpmSensor->setRpm(SPN_PUMP_STD_RPM - SPN_PUMP_RPM_DEVI_CRITICAL + 1);
  node->update();
  assertAllFlags(node->getAlarmReason(), SPN_ALARM_PUMP_RPM_CRITICAL,
         "RPM still critical, did not reach variance margin.");

  rpmSensor->setRpm(SPN_PUMP_STD_RPM - SPN_PUMP_RPM_DEVI_CRITICAL + SPN_PUMP_RPM_DEVI_VARIANCE + 1);
  node->update();
  assert(node->getAlarmReason() & SPN_ALARM_PUMP_RPM_CRITICAL, SPN_ALARM_NO_ALARM,
         "RPM not critical anymore, reached variance margin.");

  // going down from critical to technical rpm problem
  rpmSensor->setRpm(SPN_PUMP_STD_RPM - SPN_PUMP_RPM_DEVI_TECHNICAL + 2);
  node->update();
  assertAllFlags(node->getAlarmReason(), SPN_ALARM_PUMP_RPM_TECHNICAL,
         "RPM still technical, did not reach variance margin.");

  rpmSensor->setRpm(SPN_PUMP_STD_RPM - SPN_PUMP_RPM_DEVI_TECHNICAL + SPN_PUMP_RPM_DEVI_VARIANCE + 1);
  node->update();
  assert(node->getAlarmReason() & SPN_ALARM_PUMP_RPM_TECHNICAL, SPN_ALARM_NO_ALARM,
         "RPM still technical, reached variance margin.");

}
