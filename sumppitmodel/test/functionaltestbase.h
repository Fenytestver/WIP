#ifndef FUNCTIONALTESTBASE_H
#define FUNCTIONALTESTBASE_H

#include "sumppitinputs.h"
#include "sumppitnode.h"
#include "stubs.cpp"
#include "debug_util.h"
#include "multipump.h"
#include "waterlevelsensor.h"
#include "testbase.h"
#include "localview.h"
#include "floatswitch.h"

// TODO: rename this to functional test base.
class FunctionalTestBase : public TestBase
{
  public:
    FunctionalTestBase(string _name);
    virtual ~FunctionalTestBase();

    StubSystemTime* systemTime;
    StubLeakSensor* leakSensor;

    StubWaterLevelSensor* waterLevelSensor;

    StubRpmSensor *rpmSensor;
    StubVoltageSensor* voltageSensor1;
    StubVoltageSensor* voltageSensor2;
    StubVoltageSensor* systemVoltage;
    MultiPump* pump;
    Pump* subPump1;
    Pump* subPump2;
    SumpPitSensor* sensor;
    SumpPitInputs* inputs;
    StubShutoffValve* shutoffValve;
    LocalView* localView;

    StubDisplay* display;

    StubButton* disarmButton;
    StubButton* maintenanceButton;
    StubButton* armResetButton;

    StubSiren* siren;
    SumpPitNode* node;
    StubLed* ledRed;
    StubLed* ledGreen;
    StubLed* ledYellow;
    FloatSwitch * floatSwitch;

  protected:
    virtual void create();
    virtual void test();
    virtual void destroy();
};


#endif // FUNCTIONALTESTBASE_H
