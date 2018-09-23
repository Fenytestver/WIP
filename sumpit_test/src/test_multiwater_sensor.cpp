#include "test_multiwater_sensor.h"

test_multiwater_sensor::test_multiwater_sensor()
  : TestBase("Multiwater sensor unit test")
{
  //ctor
}

test_multiwater_sensor::~test_multiwater_sensor()
{
  //dtor
}

void test_multiwater_sensor::test()
{
  MultiWaterSensor* sensor = new MultiWaterSensor();
  StubWaterLevelSensor* level1 = new StubWaterLevelSensor();
  StubWaterLevelSensor* level2 = new StubWaterLevelSensor();
  sensor->addSensor(level1);
  sensor->addSensor(level2);
  level1->setLevel(SPN_WATER_LEVEL_UNKNOWN);
  level2->setLevel(SPN_WATER_LEVEL_UNKNOWN);
  assert((sensor->checkState() & SPN_ALARM_SYSTEM_ERROR) != 0, "Invalid sub sensor readings should result in a system error.");

  level1->setLevel(50);
  assert(sensor->measureLevel(), 50, "Should give the only available reading.");
  assert(sensor->checkState(), SPN_ALARM_NO_ALARM, "No alarm expected, we have a valid reading.");
  level2->setLevel(70);
  assert(sensor->measureLevel(), ((50+70)/2), "Average reading expected");

}
