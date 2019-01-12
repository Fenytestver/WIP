#include "test_normaloperation.h"

test_normaloperation::test_normaloperation() :
  FunctionalTestBase("Normal operation")
{
  //ctor
}


void test_normaloperation::test()
{
  assert(node->getMode(), SPN_INITIALIZING, "Mode should be initializing");
  node->setup();
  assert(node->getMode(), SPN_ARMED, "Mode is armed after startup");
  assertFalse(siren->isOn(), "Siren must be off after startup");

  assertFalse(shutoffValve->isActive(), "Shut off valve should be inactive at startup.");

  // simulate 80% water level.
  waterLevelSensor->setLevel(SPN_WATER_HIGH);
  voltageSensor1->setVoltage(12.0);
  voltageSensor2->setVoltage(0.0);
  rpmSensor->setRpm(SPN_PUMP_STD_RPM);
  node->update();
  assert(subPump1->isTurnedOn(), "Pump must turn on at this water level.");

  systemTime->addTime(SPN_PUMP_SPINUP_TIME - 1);
  node->update();
  assert(pump->checkState(), SPN_ALARM_NO_ALARM, "No alarm should be reported.");

  // simulate an additional leak as well.
  leakSensor->setLeaking(true);
  node->update();
  assert(siren->isOn(), "Siren must be on when leaking.");
  assert(node->getAlarmReason(), SPN_ALARM_LEAK, "Alarm reason should be leak.");

  // turn off leak
  leakSensor->setLeaking(false);
  waterLevelSensor->setLevel(SPN_WATER_LOW + 1);
  voltageSensor1->setVoltage(0.0);
  node->update();
  assertFalse(siren->isOn(), "Siren should be off");
  assertFalse(subPump1->isTurnedOn(), "Pump should turn off at low water.");
  assert(node->getAlarmReason() & SPN_ALARM_WATER_LOW, 0, "Water not low yet.");
  // should start to detect low water
  waterLevelSensor->setLevel(SPN_WATER_LOW);

  // turn on pump to detect too-low water.
  voltageSensor1->setVoltage(12.0);
  node->update();
  assertAllFlags(node->getAlarmReason(), SPN_ALARM_WATER_LOW, "Must detect low water.");
  // move water to low + variance (almost ok, but still not)
  waterLevelSensor->setLevel(SPN_WATER_LOW + SPN_WATER_VARIANCE);
  node->update();
  assertAllFlags(node->getAlarmReason(), SPN_ALARM_WATER_LOW, "Must still detect low water, below variance.");
  // move water to low + variance + a little
  waterLevelSensor->setLevel(SPN_WATER_LOW + SPN_WATER_VARIANCE + 1);
  node->update();
  assertAllFlags(node->getAlarmReason() & SPN_ALARM_WATER_LOW, 0, "Water above minimum + variance.");
  voltageSensor1->setVoltage(0);
  node->disarm();
  assertFalse(siren->isOn(), "Siren must be off after startup");
  assert(node->getMode(), SPN_DISARMED, "Mode is disarmed");
  assertFalse(shutoffValve->isActive(), "Shut off valve should be inactive at disarm.");
}
