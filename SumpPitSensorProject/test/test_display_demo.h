#include <iostream>
#include "testbase.h"
#include "localview.h"
#include "debug_util.h"

#ifndef TEST_DISPLAY_DEMO_H
#define TEST_DISPLAY_DEMO_H


class test_display_demo : public TestBase
{
  public:
    test_display_demo();
    virtual ~test_display_demo();
    virtual void test();

  protected:

  private:
};

#endif // TEST_DISPLAY_DEMO_H
