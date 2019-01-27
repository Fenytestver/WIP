#ifndef ONBUTTONPRESSLISTENER_H
#define ONBUTTONPRESSLISTENER_H


class OnButtonPressListener
{
  public:
    /** Default constructor */
    OnButtonPressListener();
    /** Default destructor */
    virtual ~OnButtonPressListener();

    virtual void onPress();
  protected:

  private:
};

#endif // ONBUTTONPRESSLISTENER_H
