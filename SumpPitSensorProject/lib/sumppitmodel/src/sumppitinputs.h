#ifndef SUMPPITINPUTS_H
#define SUMPPITINPUTS_H

#include "button.h"

class SumpPitInputs
{
  public:
    /** Default constructor */
    SumpPitInputs(Button* _disarmButton, Button* _maintenanceButton, Button* _armResetButton);
    /** Default destructor */
    virtual ~SumpPitInputs();
    virtual void setup();
    Button* disarmButton;
    Button* maintenanceButton;
    Button* armResetButton;
    virtual void update();

  private:
};

#endif // SUMPPITINPUTS_H
