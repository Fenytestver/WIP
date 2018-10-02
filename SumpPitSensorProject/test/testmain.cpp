#include "testmain.h"
#include "localview.h"

int main() {
  TestMain* testMain = new TestMain();
  testMain->run();

  cout << endl;
  printf(SPN_DISPLAY_WELCOME);
  cout << endl << "01234567890123456789" << endl << endl;
  printf(SPN_DISPLAY_NORMAL, 112, 47, "ON", "1124", "OFF", "---");
  cout << endl << "01234567890123456789" << endl << endl;
  printf(SPN_DISPLAY_MAINTENANCE, 112, 47, "ON", "1124", "OFF", "---");
  cout << endl << "01234567890123456789" << endl << endl;
  printf(SPN_DISPLAY_MAINTENANCE_REMINDER);
  cout << endl << "01234567890123456789" << endl << endl;
  printf(SPN_DISPLAY_OFF);
  cout << endl << "01234567890123456789" << endl << endl;
  printf(SPN_DISPLAY_WARNING, "CRITICAL alert", 666, "Water intrusion", "Hold RED BUTTON", "for 30sec to DISARM.");
  cout << endl << "01234567890123456789" << endl << endl;
}
void TestMain::disp(string str)
{

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
  start(new test_pump_functional());
  start(new test_pump_failure_functional());
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
