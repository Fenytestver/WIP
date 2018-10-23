#include "spn_config.h"

int spn::abso(int a) {
  return a >= 0 ? a : -1*a;
}

void spn::sb(int& flag, int mask, bool condition) {
  if (condition) {
    flag |= mask;
  } else {
    flag &= ~mask;
  }
}


bool spn::isCritical(int reason)
{
  return (reason & SPN_ALERT_ALL_CRITICAL) != SPN_ALERT_NO_ALERT;
}

bool spn::isTechnical(int reason)
{
  return (reason & SPN_ALERT_ALL_TECHNICAL) != SPN_ALERT_NO_ALERT;
}
