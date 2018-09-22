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
  pump->turnOn();
  assert(p1->isTurnedOn(), "P1 Should be turned on");
  assertFalse(p2->isTurnedOn(), "21 Shouldn't be turned on");
  pump->turnOff();
  pump->turnOn();
  assertFalse(p1->isTurnedOn(), "P1 Shouldn't be turned on");
  assert(p2->isTurnedOn(), "P2 Should be turned on");
  pump->turnOff();

  // once more
  pump->turnOn();
  assert(p1->isTurnedOn(), "P1 Should be turned on again.");
  assertFalse(p2->isTurnedOn(), "21 Shouldn't be turned on again");
  pump->turnOff();
}
