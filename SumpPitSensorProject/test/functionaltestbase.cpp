#include "functionaltestbase.h"

FunctionalTestBase::FunctionalTestBase(string _name)
  : TestBase(_name)
{
  //ctor
}

FunctionalTestBase::~FunctionalTestBase()
{
  //dtor
}

void FunctionalTestBase::create()
{
  TestBase::create();
  leakSensor = new StubLeakSensor();
  waterLevelSensor = new StubWaterLevelSensor();
  rpmSensor = new StubRpmSensor();
  voltageSensor1 = new StubVoltageSensor();
  voltageSensor2 = new StubVoltageSensor();
  systemTime = new StubSystemTime();
  // time should not be 0, that would be a problem.
  systemTime->setTime(1L);
  subPump1 = new Pump(systemTime, rpmSensor, voltageSensor1);
  subPump2 = new Pump(systemTime, rpmSensor, voltageSensor2);
  pump = new MultiPump();
  pump->addPump(subPump1);
  pump->addPump(subPump2);

  disarmButton = new StubButton(systemTime);
  maintenanceButton = new StubButton(systemTime);
  armResetButton = new StubButton(systemTime);

  sensor = new SumpPitSensor(waterLevelSensor, 1, leakSensor, 1, pump);
  inputs = new SumpPitInputs(disarmButton, maintenanceButton, armResetButton);
  siren = new StubSiren();
  display = new StubDisplay();
  shutoffValve = new StubShutoffValve();
  node = new SumpPitNode(siren, display, sensor, inputs, shutoffValve);
}

void FunctionalTestBase::destroy()
{
  TestBase::destroy();
}

void FunctionalTestBase::test()
{
  TestBase::test();
}
