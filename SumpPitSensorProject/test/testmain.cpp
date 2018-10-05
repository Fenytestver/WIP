#include "testmain.h"
#include "localview.h"

int main() {
  TestMain* testMain = new TestMain();
  testMain->run();
}

TestMain::TestMain()
{
  //ctor
}

TestMain::~TestMain()
{
  //dtor
}

void TestMain::run()
{
  success = 0;
  fail = 0;
  runSmallTests();
  runFunctionalTests();
  if (fail == 0) {
    cout << endl << "# All " << success << " tests finished successfully." << endl;
  } else {
    cout << endl << "# Test failed: " << fail << ", successful: " << success << endl;
  }
}
void TestMain::runFunctionalTests()
{
  start(new test_normaloperation());
  start(new test_pump_failure_functional());
  // LCD display samples
  start(new test_display_demo());
}

void TestMain::runSmallTests()
{
  start(new test_pump_unit());
  start(new test_multipump_unit());
  start(new test_multipump_unit());
}

void TestMain::start(TestBase* testCase)
{
  if (testCase->run()) {
    success++;
  } else {
    fail++;
  }
}
