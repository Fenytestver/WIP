#ifndef TESTBASE_H
#define TESTBASE_H

#include "sumppitinputs.h"
#include "sumppitnode.h"
#include "stubs.cpp"
#include "debug_util.h"
#include "multipump.h"
// TODO: rename this to functional test base.
class TestBase
{
  public:
    TestBase(string _name);
    virtual ~TestBase();

    StubSystemTime* systemTime;
    StubLeakSensor* leakSensor;
    StubWaterLevelSensor* waterLevelSensor;

    StubRpmSensor *rpmSensor;
    StubVoltageSensor* voltageSensor;
    MultiPump* pump;
    Pump* subPump1;
    Pump* subPump2;
    SumpPitSensor* sensor;
    SumpPitInputs* inputs;

    StubDisplay* display;

    StubButton* armButton;
    StubButton* maintenanceButton;
    StubButton* resetButton;
    StubSiren* siren;
    SumpPitNode* node;

    bool run();
  protected:
    void create();
    virtual void test();
    void destroy();

    void fail(string message);
  private:
    string name;
};

#endif // TESTBASE_H
