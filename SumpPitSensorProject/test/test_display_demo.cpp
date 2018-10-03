#include "test_display_demo.h"

test_display_demo::test_display_demo() :
  TestBase("Display a few samples of the LCD screen")
{
  //ctor
}

test_display_demo::~test_display_demo()
{
  //dtor
}

void test_display_demo::test()
{
  cout << endl;
  printf(SPN_DISPLAY_WELCOME);
  cout << endl << "01234567890123456789" << endl << endl;
  // Normal Operation, 112", 47%, Pump1:ON, Pump2 RPM: 1124, Pump2 OFF, Pump2 RPM: ---
  printf(SPN_DISPLAY_NORMAL, 112, 47, "ON", "1124", "OFF", "---");
  cout << endl << "01234567890123456789" << endl << endl;
  // Maintenance, 112", 47%, Pump1:ON, Pump2 RPM: 1124, Pump2 OFF, Pump2 RPM: ---
  printf(SPN_DISPLAY_MAINTENANCE, 112, 47, "ON", "1124", "OFF", "---");
  cout << endl << "01234567890123456789" << endl << endl;
  // maintenance message
  printf(SPN_DISPLAY_MAINTENANCE_REMINDER);
  cout << endl << "01234567890123456789" << endl << endl;
  // "off" message
  printf(SPN_DISPLAY_OFF);
  cout << endl << "01234567890123456789" << endl << endl;
  // Critical alert message, error code: 666, 3x text message
  printf(SPN_DISPLAY_WARNING, "CRITICAL alert", 666, "Water intrusion", "Hold RED BUTTON", "for 30sec to DISARM.");
  cout << endl << "01234567890123456789" << endl << endl;
}
