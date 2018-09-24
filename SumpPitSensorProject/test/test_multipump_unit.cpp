#include "test_multipump_unit.h"

test_multipump_unit::test_multipump_unit()
  : TestBase("Multi pump unit test")
{
  //ctor

}

test_multipump_unit::~test_multipump_unit()
{
  //dtor
}

void test_multipump_unit::test()
{
  RpmSensor* rpmSensor = new StubRpmSensor();
  StubSystemTime* systemTime = new StubSystemTime();

  VoltageSensor* voltageSensor = new StubVoltageSensor();
  systemTime->setTime(1L);
  Pump* subPump1 = new Pump(systemTime, rpmSensor, voltageSensor);
  Pump* subPump2 = new Pump(systemTime, rpmSensor, voltageSensor);
  MultiPump* pump = new MultiPump();

  pump->addPump(subPump1);
  pump->addPump(subPump2);
  pump->turnOn();

  assert(subPump1->isTurnedOn(), "P1 Should be turned on");
  assertFalse(subPump2->isTurnedOn(), "P2 Shouldn't be turned on");
  pump->turnOff();
  pump->turnOn();
  assertFalse(subPump1->isTurnedOn(), "P1 Shouldn't be turned on");
  assert(subPump2->isTurnedOn(), "P2 Should be turned on");
  pump->turnOff();

  // once more
  pump->turnOn();
  assert(subPump1->isTurnedOn(), "P1 Should be turned on again.");
  assertFalse(subPump2->isTurnedOn(), "21 Shouldn't be turned on again");
  pump->turnOff();
  assertFalse(subPump1->isTurnedOn(), "P1: All off");
  assertFalse(subPump2->isTurnedOn(), "P2: All off");

  // turbo
  pump->turbo();
  assert(subPump1->isTurnedOn(), "P1: All on should be on during turbo");
  assert(subPump2->isTurnedOn(), "P2: All on should be on during turbo");
}
