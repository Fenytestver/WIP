#include "debug_util.h"
void spn::assert(bool b, string msg) {
  if (!b) {
    std::cerr << "\t\tFail: " << msg << "\n";
    throw 100;
  } else {
    std::cout << "\t\tOk: \'" << msg << "\'\n";
  }
}

void spn::assert(long actual, long expected, string msg) {
  if (expected != actual) {
    std::cerr << "\t\tFail: " << msg << ", expected:" << expected << ", actual:" << actual << "\n";
    throw 100;
  } else {
    std::cout << "\t\tOk: \'" << msg << "\'\n";
  }
}
void spn::assertFalse(bool b, string msg) {
  assert(!b, msg);
}
