#include "testbase.h"

TestBase::TestBase(string _name)
{
  //ctor
  name = _name;
}

TestBase::~TestBase()
{
  //dtor
}

bool TestBase::run()
{
  bool successful = false;
  try {
    std::cout << endl << "\t" << name << ": create" << endl;
    create();
    std::cout << "\t" << name << ": start test" << endl;
    test();
    std::cout << "\t" << name << ": cleanup" << endl;
    destroy();
    std::cout << "\t" << name << ": finished" << endl;
    successful = true;
  } catch (int e) {
    std::cout << "\t" << name << ": error running test (" << e << ")" << endl;
  }

  return successful;
}

void TestBase::create()
{
  leakSensor = new StubLeakSensor();
  waterLevelSensor = new StubWaterLevelSensor();
  rpmSensor = new StubRpmSensor();
  voltageSensor = new StubVoltageSensor();
  systemTime = new StubSystemTime();
  // time should not be 0, that would be a problem.
  systemTime->setTime(1L);

  p1 = new Pump(systemTime, rpmSensor, voltageSensor);
  p2 = new Pump(systemTime, rpmSensor, voltageSensor);
  pump = new MultiPump();
  pump->addPump(p1);
  pump->addPump(p2);

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

void TestBase::destroy()
{
  // TODO: destroy?
}


void TestBase::test()
{
  // no-op. add implementation to subclass
}

void TestBase::fail(string message)
{
  cout << "\t" << "Test \"" << name << "\" failed: " << message << endl;
  throw -1;
}
