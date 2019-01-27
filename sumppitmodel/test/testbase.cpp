#include "testbase.h"

TestBase::TestBase(string _name)
{
  //ctor
  name = _name;
}

TestBase::~TestBase()
{
  //dtor
}

bool TestBase::run()
{
  bool successful = false;
  try {
    std::cout << endl << "\t" << name << ": create" << endl;
    create();
    std::cout << "\t" << name << ": start test" << endl;
    test();
    std::cout << "\t" << name << ": cleanup" << endl;
    destroy();
    std::cout << "\t" << name << ": finished" << endl;
    successful = true;
  } catch (int e) {
    std::cout << "\t" << name << ": error running test (" << e << ")" << endl;
  }

  return successful;
}

void TestBase::create()
{

}

void TestBase::test()
{

}

void TestBase::destroy()
{

}

void TestBase::fail(string message)
{
  cout << "\t" << "Test \"" << name << "\" failed: " << message << endl;
  throw -1;
}
