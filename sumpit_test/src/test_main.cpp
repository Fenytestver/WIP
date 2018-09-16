#include "test_main.h"
#include "led.h"
#include "sumppitsensor.h"
#include "sumppitinputs.h"
#include "pump.h"

#include <iostream>

int main()
{
    std::cout << "Start";
    test_main* m = new test_main();

    std::cout << "End";
}
test_main::test_main()
{
  std::cout << "test main";

  leakSensor = new StubLeakSensor();
  waterLevelSensor = new StubWaterLevelSensor();
  rpmSensor = new StubRpmSensor();
  voltageSensor = new StubVoltageSensor();
  pump = new Pump(rpmSensor, voltageSensor);

  StubButton* disarmButton = new StubButton();
  StubButton* maintenanceButton = new StubButton();
  StubButton* armResetButton = new StubButton();

  // FIXME: only one water level and one leak..
  sensor = new SumpPitSensor(waterLevelSensor, 1, leakSensor, 1, pump);
  inputs = new SumpPitInputs(disarmButton, maintenanceButton, armResetButton);
  siren = new StubSiren();
  display = new StubDisplay();
  inputs = new SumpPitInputs(armButton, maintenanceButton, resetButton);
  node = new SumpPitNode(siren, display, sensor, inputs);
}

test_main::~test_main()
{}
