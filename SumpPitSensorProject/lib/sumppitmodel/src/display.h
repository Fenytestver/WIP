#ifndef DISPLAY_H
#define DISPLAY_H

#include "state.h"
class Display
{
  public:
    /** Default constructor */
    Display();
    /** Default destructor */
    virtual ~Display();
    virtual void setup();
    virtual void displayMessage(char* message);
    virtual void clear();
    virtual void show(State state);
  protected:

  private:
};

#endif // DISPLAY_H
