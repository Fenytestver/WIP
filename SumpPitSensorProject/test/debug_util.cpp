#include "debug_util.h"
void spn::assert(bool b, string msg) {
  if (!b) {
    SPN_DBG_FAIL << "Fail: " << msg << "\n";
    throw 100;
  } else {
    SPN_DBG_SUCCESS << "Test \'" << msg << "\': OK\n";
  }
}

void spn::assert(long actual, long expected, string msg) {
  if (expected != actual) {
    SPN_DBG_FAIL << "Fail: " << msg << ", expected:" << expected << ", actual:" << actual << "\n";
    throw 100;
  } else {
    SPN_DBG_SUCCESS << "Ok: \'" << msg << "\'\n";
  }
}
void spn::assertFalse(bool b, string msg) {
  assert(!b, msg);
}

void spn::assertAllFlags(int value, int mask, string msg) {
  if ((value & mask) != mask) {
    bitset<8> valBits(value);
    bitset<8> maskBits(mask);
    SPN_DBG_FAIL << "Fail: " << msg << ", minimum:" << maskBits << ", actual:" << valBits << "\n";
    throw 100;
  } else {
    SPN_DBG_SUCCESS << "Ok: \'" << msg << "\'\n";
  }
}
