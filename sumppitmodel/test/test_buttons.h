#include <iostream>
#include "testbase.h"
#include "button.h"
#include "onbuttonpresslistener.h"
#include "stubs.cpp"
#ifndef TEST_BUTTONS_H
#define TEST_BUTTONS_H


class test_buttons : public TestBase
{
  public:
    test_buttons();
    virtual ~test_buttons();
    virtual void test();

  protected:


  private:

    class MockOnClick : public OnButtonPressListener {
      public:
        MockOnClick(test_buttons* _test) {
          test = _test;
        }
        void onPress() {
          test->clicked = true;
        }
      private:
        test_buttons* test;
    };

    class MockOnLongClick : public OnButtonPressListener {

      public:
        MockOnLongClick(test_buttons* _test) {
          test = _test;
        }
        void onPress() {
          test->longClicked = true;
        }
      private:
        test_buttons* test;
    };

    MockOnClick* onClickListener;
    MockOnLongClick* onLongClickListener;

    bool clicked;
    bool longClicked;
};

#endif // TEST_BUTTONS_H
