#ifndef TEST_MAIN_H
#define TEST_MAIN_H
#include "sumppitinputs.h"
#include "sumppitnode.h"
#include "stubs.cpp"

class test_engine
{
    public:
      /** Default constructor */
      test_engine();
      /** Default destructor */
      virtual ~test_engine();

      StubLeakSensor* leakSensor;
      StubWaterLevelSensor* waterLevelSensor;

      RpmSensor *rpmSensor;
      VoltageSensor* voltageSensor;
      Pump* pump;
      SumpPitSensor* sensor;
      SumpPitInputs* inputs;

      StubDisplay* display;

      StubButton* armButton;
      StubButton* maintenanceButton;
      StubButton* resetButton;
      StubSiren* siren;
      SumpPitNode* node;

      void test_first();
    protected:

    private:
  //    SumpPitSensor* sumpPitSensor;
};

#endif // TEST_MAIN_H
