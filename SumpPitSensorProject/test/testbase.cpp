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
  multiWaterSensor = new MultiWaterSensor();
  waterLevelSensor1 = new StubWaterLevelSensor();
  waterLevelSensor2 = new StubWaterLevelSensor();
  multiWaterSensor->addSensor(waterLevelSensor1);
  multiWaterSensor->addSensor(waterLevelSensor2);
  rpmSensor = new StubRpmSensor();
  voltageSensor = new StubVoltageSensor();
  systemTime = new StubSystemTime();
  // time should not be 0, that would be a problem.
  systemTime->setTime(1L);

  subPump1 = new Pump(systemTime, rpmSensor, voltageSensor);
  subPump2 = new Pump(systemTime, rpmSensor, voltageSensor);
  pump = new MultiPump();
  pump->addPump(subPump1);
  pump->addPump(subPump2);

  disarmButton = new StubButton();
  maintenanceButton = new StubButton();
  armResetButton = new StubButton();

  sensor = new SumpPitSensor(multiWaterSensor, 1, leakSensor, 1, pump);
  inputs = new SumpPitInputs(disarmButton, maintenanceButton, armResetButton);
  siren = new StubSiren();
  display = new StubDisplay();
  node = new SumpPitNode(siren, display, sensor, inputs);
}

void TestBase::destroy()
{
  /*delete leakSensor;
  delete multiWaterSensor;
  delete waterLevelSensor1;
  delete waterLevelSensor2;

  delete rpmSensor;
  delete voltageSensor;
  delete systemTime;

  delete systemTime;

  delete pump;
  delete subPump1;
  delete subPump2;

  */
  delete node;
  delete sensor;
  delete inputs;
  delete display;
  delete siren;
  delete disarmButton;
  delete maintenanceButton;
  delete armResetButton;
  delete waterLevelSensor1;
  delete waterLevelSensor2;
  //delete systemTime;
  // FIXME: delete (free) members.
  //delete subPump1;
  //delete subPump2;
  //delete pump;

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
