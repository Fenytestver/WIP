#include <iostream>
#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H

#define SPN_DBG_MSG std::cout << "\t\t "
#define SPN_DBG_STUB std::cout << "\t\t>"
#define SPN_DBG_SUCCESS std::cout << "\t\t"
#define SPN_DBG_FAIL std::cout << "\t\t"

using namespace std;
namespace spn {
  void assert(bool b, string msg);
  void assert(long actual, long expected, string msg);
  void assertFalse(bool b, string msg);
}

#endif // DEBUG_UTIL_H
