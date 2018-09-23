#ifndef BUTTON_H
#define BUTTON_H
#include "onbuttonpresslistener.h"

class Button
{
  public:
    /** Default constructor */
    Button();
    /** Default destructor */
    virtual ~Button();

    void setOnButtonPressListener(OnButtonPressListener* onPressListener) {
      _onPressListener = onPressListener;
    }

  protected:
    OnButtonPressListener* _onPressListener;

  private:
};

#endif // BUTTON_H
