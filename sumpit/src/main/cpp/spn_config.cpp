#include "spn_config.h"

int spn::abs(int a) {
  return a >= 0 ? a : -1*a;
}

void spn::sb(int& flag, int mask, bool condition) {
  if (condition) {
    flag |= mask;
  } else {
    flag &= ~flag;
  }
}
