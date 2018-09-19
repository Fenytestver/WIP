#include "test_pump_functional.h"

test_pump_functional::test_pump_functional()
  : TestBase("Pump functions")
{
  //ctor
}

test_pump_functional::~test_pump_functional()
{
  //dtor
}

void test_pump_functional::test()
{
node->setup();
  waterLevelSensor->setLevel(SPN_WATER_LOW - 1);
  node->update();
  assertFalse(pump->isTurnedOn(), "Pump should not be on, water too low.");

  waterLevelSensor->setLevel(SPN_WATER_HIGH - 1);
  node->update();
  assertFalse(pump->isTurnedOn(), "Pump should not be on, water too low.");

  waterLevelSensor->setLevel(SPN_WATER_HIGH + 1);
  node->update();
  assert(pump->isTurnedOn(), "Pump should on, water above high level.");

  waterLevelSensor->setLevel(SPN_WATER_HIGH - 10);
  node->update();
  assertFalse(pump->isTurnedOn(), "Pump should be still on.");

  waterLevelSensor->setLevel(SPN_WATER_LOW - 1);
  node->update();
  assertFalse(pump->isTurnedOn(), "Pump turn off here.");
}
