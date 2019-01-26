#ifndef TESTBASE_H
#define TESTBASE_H

#include "debug_util.h"

class TestBase
{
  public:
    TestBase(string _name);
    virtual ~TestBase();
    bool run();

  protected:
    virtual void create();
    virtual void test();
    virtual void destroy();

    void fail(string message);
  private:
    string name;
};

#endif // TESTBASE_H
