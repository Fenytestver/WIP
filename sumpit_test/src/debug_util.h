#include <iostream>
#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H

using namespace std;
namespace spn {
  void assert(bool b, string msg);
  void assert(long actual, long expected, string msg);
  void assertFalse(bool b, string msg);
}

#endif // DEBUG_UTIL_H
