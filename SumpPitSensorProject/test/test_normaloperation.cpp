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
  node->update();
  assert(subPump1->isTurnedOn(), "Pump must turn on at this water level.");
  systemTime->addTime(10000);
  node->update();
  assert(pump->checkState(), SPN_ALARM_NO_ALARM, "No alarm should be reported.");

  // simulate an additional leak as well.
  leakSensor->setLeaking(true);
  node->update();
  assert(siren->isOn(), "Siren must be on when leaking.");
  assert(node->getAlarmReason(), SPN_ALARM_LEAK, "Alarm reason should be leak.");

  leakSensor->setLeaking(false);
  waterLevelSensor->setLevel(SPN_WATER_LOW - 1);
  voltageSensor1->setVoltage(0.0);
  node->update();
  assertFalse(siren->isOn(), "Siren should be off");
  assertFalse(subPump1->isTurnedOn(), "Pump should turn off at low water.");

  node->disarm();
  assertFalse(siren->isOn(), "Siren must be off after startup");
  assert(node->getMode(), SPN_DISARMED, "Mode is armed after startup");
  assertFalse(shutoffValve->isActive(), "Shut off valve should be inactive at disarm.");
}
