#ifndef LOCALVIEW_H
#define LOCALVIEW_H

#define SPN_DISPLAY_TEST \
  "####################\n"\
  "####################\n"\
  "####################\n"\
  "####################"

#define SPN_DISPLAY_WELCOME \
  "      Welcome!     \n"\
  "   Initializing... \n"\
  "                   \n"\
  "     Please wait!  "

/**
Normal mode display.

int(3) level(in)
int(3) level(percent)
string(3) pump1(status)
string(max 6) pump1(rpm)
string(3) pump2(status)
string(max 6) pump2(rpm)
*/
#define SPN_DISPLAY_NORMAL \
  "%s\n"\
  "Level:%3d\" - %3d%%\n"\
  "Pump1:%s rpm:%s\n"\
  "Pump2:%s rpm:%s"
// 01234567890123456789

#define SPN_DISPLAY_NORMAL_MODE_TEXT "**NORMAL OPERATION**"
#define SPN_DISPLAY_MAINTENANCE_MODE_TEXT "--MAINTENANCE MODE--"

/**
warning screen with status, error code and 3 lines for text
string(?) mode
int(?) error-code
string(?) line 1
string(?) line 2
string(?) line 3
*/
#define SPN_DISPLAY_WARNING \
  "%s! #%s\n"\
  "%s\n"\
  "%s\n"\
  "%s"
// 01234567890123456789
#define SPN_DISPLAY_OFF \
  "-All ALARMS ARE OFF-\n"\
  "The Water Intrusion\n"\
  "Systeem is disabled!\n"\
  "-All ALARMS ARE OFF-\n"
// 01234567890123456789
#define SPN_DISPLAY_MAINTENANCE_REMINDER \
  "Maintenance mode ON!\n"\
  "       REARM\n"\
  "       REARM\n"\
  "when job is complete\n"
// 01234567890123456789

#include "state.h"
#include "display.h"
#include "spn_config.h"

//#ifdef SPN_UNIT_TEST
#include <cstdio>
//#endif // SPN_UNIT_TEST
#include "pump.h"
#include "waterlevelsensor.h"
using namespace spn;

class LocalView
{
  public:
    LocalView(Display* _display);
    virtual ~LocalView();
    virtual void setup();
    virtual void render(State state);

  protected:
  private:
    char* pump1Rpm;
    char* pump2Rpm;
    char* statusString;
    char* lines[4];
    char* message;

    Display* display;
    void renderArmed(State state);

};

#endif // LOCALVIEW_H
