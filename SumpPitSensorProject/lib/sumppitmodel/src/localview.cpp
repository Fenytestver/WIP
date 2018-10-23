#include "localview.h"
#include "spn_config.h"

LocalView::LocalView(Display* _display)
{
  //ctor
  display = _display;
}

LocalView::~LocalView()
{
  //dtor
}

void LocalView::setup()
{
  display->setup();
}

void LocalView::render(State state)
{
  switch (state.mode) {
    case Mode::SPN_ARMED:
      renderArmed(state);
      break;
    case Mode::SPN_DISARMED:
      display->displayMessage(SPN_DISPLAY_OFF);
      break;
    case Mode::SPN_INITIALIZING:
      display->displayMessage(SPN_DISPLAY_WELCOME);
      break;
    case Mode::SPN_MAINTENANCE:
      display->displayMessage(SPN_DISPLAY_MAINTENANCE);
      break;
  }
}

void LocalView::renderArmed(State state)
{
  display->displayMessage(SPN_DISPLAY_WARNING);
  if (isCritical(state.alarmReason)) {

  }
}
