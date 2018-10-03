#ifndef LOCALVIEW_H
#define LOCALVIEW_H

#define SPN_DISPLAY_TEST \
  "####################\n"\
  "####################\n"\
  "####################\n"\
  "####################"

#define SPN_DISPLAY_WELCOME \
  "      Welcome!      \n"\
  "   Initializing...  \n"\
  "                    \n"\
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
  "**NORMAL OPERATION**\n"\
  "Level:%3d\" - %3d%%\n"\
  "Pump1:%s rpm:%s\n"\
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
  "%s! #%3d\n"\
  "%s\n"\
  "%s\n"\
  "%s"

/**
same as normal.
*/
// 01234567890123456789
#define SPN_DISPLAY_MAINTENANCE \
  "--MAINTENANCE MODE--\n"\
  "Level:%3d\" - %3d%%\n"\
  "Pump1:%s rpm:%s\n"\
  "Pump2:%s rpm:%s"
// 01234567890123456789
#define SPN_DISPLAY_OFF \
  "-All ALARMS ARE OFF-\n"\
  "The Water Intrusion\n"\
  "Systeem is disabled!\n"\
  "-All ALARMS ARE OFF-"
// 01234567890123456789
#define SPN_DISPLAY_MAINTENANCE_REMINDER \
  "Maintenance mode ON!\n"\
  "       REARM\n"\
  "       REARM\n"\
  "when job is complete"
// 01234567890123456789

class LocalView
{
  public:
    LocalView();
    virtual ~LocalView();
    virtual char* render();

  protected:

  private:
};

#endif // LOCALVIEW_H
