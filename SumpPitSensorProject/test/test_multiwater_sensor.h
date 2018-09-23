#ifndef TEST_MULTIWATER_SENSOR_H
#define TEST_MULTIWATER_SENSOR_H
#include "testbase.h"
#include "stubs.cpp"
#include "multiwatersensor.h"

class test_multiwater_sensor : public TestBase
{
  public:
    test_multiwater_sensor();
    virtual ~test_multiwater_sensor();
    virtual void test();
  protected:

  private:
};

#endif // TEST_MULTIWATER_SENSOR_H
