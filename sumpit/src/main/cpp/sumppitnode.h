#ifndef SUMPPITNODE_H
#define SUMPPITNODE_H

#include "display.h"
#include "siren.h"
#include "sumppitinputs.h"
#include "sumppitsensor.h"

// The main system modes
enum {
  SPN_INITIALIZING, SPN_DISARMED, SPN_ARMED, SPN_SYS_ERROR
}; // end SumpPitNodeModes

class SumpPitNode
{
  public:
    /** Default constructor */
    SumpPitNode(Siren* _siren,
    Display* _display,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs);
    /** Default destructor */
    virtual ~SumpPitNode();

    /**
       Tells the currently selected mode.
       Value will be one of SumpPitNodeModes.
    */
    virtual int getMode();
    virtual void arm();
    virtual void disarm();
    virtual void maintenance();
    virtual void alarm();
  protected:
    Siren* siren;
    Display* display;
    SumpPitSensor* sensor;
    SumpPitInputs* inputs;
  private:
    int mode;
};

#endif // SUMPPITNODE_H
