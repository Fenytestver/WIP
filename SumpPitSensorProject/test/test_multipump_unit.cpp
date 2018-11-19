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

  StubVoltageSensor* voltageSensor1 = new StubVoltageSensor();
  StubVoltageSensor* voltageSensor2 = new StubVoltageSensor();
  systemTime->setTime(1L);
  Pump* subPump1 = new Pump(systemTime, rpmSensor, voltageSensor1);
  Pump* subPump2 = new Pump(systemTime, rpmSensor, voltageSensor2);
  MultiPump* pump = new MultiPump();

  pump->addPump(subPump1);
  pump->addPump(subPump2);
  voltageSensor1->setVoltage(12.0);
  voltageSensor2->setVoltage(0.0);
  State state;
  pump->update(&state);

  assert(subPump1->isTurnedOn(), "P1 Should be turned on");
  assertFalse(subPump2->isTurnedOn(), "P2 Shouldn't be turned on");

  voltageSensor1->setVoltage(0.0);
  voltageSensor2->setVoltage(12.0);
  pump->update(&state);
  assertFalse(subPump1->isTurnedOn(), "P1 Shouldn't be turned on");
  assert(subPump2->isTurnedOn(), "P2 Should be turned on");

  // turbo
  voltageSensor1->setVoltage(12.0);
  voltageSensor2->setVoltage(12.0);
  pump->update(&state);
  assert(subPump1->isTurnedOn(), "P1: All on should be on during turbo");
  assert(subPump2->isTurnedOn(), "P2: All on should be on during turbo");
}
