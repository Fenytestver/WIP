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
  node->update();
  assert(pump->isTurnedOn(), "Pump must turn on at this water level.");

  // simulate an additional leak as well.
  leakSensor->setLeaking(true);
  node->update();
  assert(siren->isOn(), "Siren must be on with 2 issues detected.");
  assert(node->getAlarmReason(), SPN_ALARM_LEAK, "Alarm reason should be leak.");

  leakSensor->setLeaking(false);
  waterLevelSensor->setLevel(SPN_WATER_LOW - 1);
  node->update();
  assertFalse(siren->isOn(), "Siren should be off");
  assertFalse(pump->isTurnedOn(), "Pump should turn off at low water.");

  node->disarm();
  assertFalse(siren->isOn(), "Siren must be off after startup");
  assert(node->getMode(), SPN_DISARMED, "Mode is armed after startup");
  assertFalse(shutoffValve->isActive(), "Shut off valve should be inactive at disarm.");
}
