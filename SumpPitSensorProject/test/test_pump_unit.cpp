#include "test_pump_unit.h"

test_pump_unit::test_pump_unit()
  : FunctionalTestBase("Pump unit test.")
{
  //ctor
}

test_pump_unit::~test_pump_unit()
{
  //dtor
}

void test_pump_unit::test() {
  assert(subPump1->getUptime(), 0L, "Pump uptime should be 0 at start.");
  long now = systemTime->nowMillis();
  voltageSensor1->setVoltage(12.0);
  pump->update();
  assert(subPump1->isTurnedOn(), "Pump turned on.");
  systemTime->addTime(1L);
  assert(systemTime->nowMillis(), now + 1L, "1ms passed since \"now\".");
  assert(subPump1->getUptime(), 1L, "1ms since we started the pump.");

  // now turn off the pump
  voltageSensor1->setVoltage(0.0);
  pump->update();
  assert(subPump1->getUptime(), 0L, "Pump turned off, no uptime (0).");
  assertFalse(subPump1->isTurnedOn(), "Pump turned off.");
}
