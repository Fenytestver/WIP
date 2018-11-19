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

long spn::mapp(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool spn::isCritical(int reason)
{
  return (reason & SPN_ALERT_ALL_CRITICAL) != SPN_ALERT_NO_ALERT;
}

bool spn::isTechnical(int reason)
{
  return (reason & SPN_ALERT_ALL_TECHNICAL) != SPN_ALERT_NO_ALERT;
}

void spn::statusToString(int status, char* out)
{
  for (int i = 0; i<9; ++i) {
    out[i] = (1 == ( (status >> i) & 1)) ? '1':'0';
  }
  out[9] = '\0';
}
