#ifndef LOCALVIEW_H
#define LOCALVIEW_H

#define SPN_DISPLAY_TEST \
  "####################"\
  "####################"\
  "####################"\
  "####################"

#define SPN_DISPLAY_WELCOME \
  "      Welcome!      "\
  "   Initializing...  "\
  "                    "\
  "     Please wait!   "

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
  "**NORMAL OPERATION**"\
  "Level:%3d\" - %3d%%"\
  "Pump1:%s rpm:%s"\
  "Pump2:%s rpm:%s"
// 01234567890123456789
/**
warning screen with status, error code and 3 lines for text
string(?) mode
int(?) error-code
string(?) line 1
string(?) line 2
string(?) line 3
*/
#define SPN_DISPLAY_WARNING \
  "%s! #%3d"\
  "%s"\
  "%s"\
  "%s"

/**
same as normal.
*/
// 01234567890123456789
#define SPN_DISPLAY_MAINTENANCE \
  "--MAINTENANCE MODE--"\
  "Level:%3d\" - %3d%%"\
  "Pump1:%s rpm:%s"\
  "Pump2:%s rpm:%s"
// 01234567890123456789
#define SPN_DISPLAY_OFF \
  "-All ALARMS ARE OFF-"\
  "The Water Intrusion"\
  "Systeem is disabled!"\
  "-All ALARMS ARE OFF-"
// 01234567890123456789
#define SPN_DISPLAY_MAINTENANCE_REMINDER \
  "Maintenance mode ON!"\
  "       REARM"\
  "       REARM"\
  "when job is complete"
// 01234567890123456789

#include "state.h"
#include "display.h"
#include "spn_config.h"

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
    Display* display;
    void renderArmed(State state);
};

#endif // LOCALVIEW_H
