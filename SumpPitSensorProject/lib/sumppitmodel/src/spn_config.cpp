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

double spn::mapp(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long spn::mapp(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool spn::isCritical(int reason)
{
  return (reason & SPN_ALERT_ALL_CRITICAL) != SPN_ALERT_NO_ALERT;
}

bool spn::isCriticalShutoff(int reason)
{
  return (reason & SPN_ALERT_ALL_CRITICAL_SHUTOFF) != SPN_ALERT_NO_ALERT;
}

bool spn::isTechnical(int reason)
{
  return (reason & SPN_ALERT_ALL_TECHNICAL) != SPN_ALERT_NO_ALERT;
}

void spn::statusToString(int status, char* out)
{
  for (int i = 0; i<SPN_STATUS_BITS; ++i) {
    out[i] = (1 == ( (status >> i) & 1)) ? '1':'0';
  }
  out[SPN_STATUS_BITS] = '\0';
}

long spn::millisToSec(long millis)
{
  return millis / 1000;
}

int spn::toPercent(int value, int complete) {
  return (int)((100.0 * (double)value / (double)complete));
}

int spn::toPercentExtra(int value, int complete) {
  return toPercent(value - complete, complete);
}
