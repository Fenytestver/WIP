#include <iostream>
#ifndef TEST_PUMP_FAILURE_LONGOPERATION_H
#define TEST_PUMP_FAILURE_LONGOPERATION_H
#include "functionaltestbase.h"

class test_pump_failure_longoperation : public FunctionalTestBase
{
  public:
    test_pump_failure_longoperation() : FunctionalTestBase("Pump long operation failure test") {

    }
    virtual void test() {
      node->setup();
      // set high water, turn on pump.
      waterLevelSensor->setLevel(SPN_WATER_HIGH);
      voltageSensor1->setVoltage(12.0);
      voltageSensor2->setVoltage(0.0);
      rpmSensor->setRpm(SPN_PUMP_STD_RPM);
      node->update();
      assert(pump->checkState(), SPN_ALARM_NO_ALARM, "No alarm should be reported.");
      systemTime->addTime(SPN_PUMP_CYCLE_MAX_LENGTH_TECHNICAL + 10);
      node->update();
      assert((pump->checkState() & SPN_ALARM_PUMP_CYCLE_TECHNICAL) != 0, "Pump alarm should be technical+long operation.");
      systemTime->setTime(SPN_PUMP_CYCLE_MAX_LENGTH_CRITICAL + 10);
      node->update();
      assert((pump->checkState() & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0, "Pump alarm should be critical+long operation.");
    }
  protected:

  private:
};

#endif // TEST_PUMP_FAILURE_LONGOPERATION_H
