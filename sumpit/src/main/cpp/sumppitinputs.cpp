#include "sumppitinputs.h"

SumpPitInputs::SumpPitInputs(Button* _disarmButton, Button* _maintenanceButton, Button* _armResetButton)
{
  //ctor
  disarmButton = _disarmButton;
  maintenanceButton = _maintenanceButton;
  armResetButton = _armResetButton;
}

SumpPitInputs::~SumpPitInputs()
{
  //dtor
}
