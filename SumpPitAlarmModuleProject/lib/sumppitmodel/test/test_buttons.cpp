#include "test_buttons.h"

test_buttons::test_buttons() :
  TestBase("Test button behavior")
{
  //ctor
  clicked = false;
  longClicked = false;
  onClickListener = new MockOnClick(this);
  onLongClickListener = new MockOnLongClick(this);
}

test_buttons::~test_buttons()
{
  //dtor
}

void test_buttons::test()
{
  StubSystemTime* systemTime = new StubSystemTime();
  systemTime->setTime(1L);
  StubButton* button = new StubButton(systemTime);
  button->setOnButtonPressListener(onClickListener);
  button->setOnButtonLongPressListener(onLongClickListener);

  // button down event
  button->setPressed(true);
  // TODO: this may be a down/up event, but then we need a separate down event for beeping.
  assert(clicked, "We pressed the button, click should be triggered.");
  button->setPressed(true);
  button->setPressed(true);
  button->setPressed(true);
  assertFalse(longClicked, "Not enough time for a [Long press] yet");

  systemTime->addTime(SPN_BUTTON_LONG_PRESS_TIME - 1);
  button->setPressed(true); // update state.
  assertFalse(longClicked, "Not enough time for a [Long press] yet, but almost.");
  systemTime->addTime(2);
  button->setPressed(true); // update state.
  assert(longClicked, "Long press should be detected after this time.");
}

