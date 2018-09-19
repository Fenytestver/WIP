#include <iostream>

#ifndef TESTMAIN_H
#define TESTMAIN_H

#include "testbase.h"
#include "test_normaloperation.h"
#include "test_pump_functional.h"
#include "test_pump_unit.h"
#include "test_pump_failure_functional.h"

class TestMain
{
  public:
    TestMain();
    virtual ~TestMain();

    void run();
  protected:

  private:
    void runFunctionalTests();
    void runSmallTests();
    // utility to start the test
    void start(TestBase* testCase);

    int success;
    int fail;
};

#endif // TESTMAIN_H
