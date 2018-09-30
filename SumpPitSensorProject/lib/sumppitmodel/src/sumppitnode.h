#ifndef SUMPPITNODE_H
#define SUMPPITNODE_H

#include "display.h"
#include "siren.h"
#include "sumppitinputs.h"
#include "sumppitsensor.h"
#include "spn_config.h"
#include "shutoffvalve.h"

using namespace spn;
// The main system modes
enum {
  SPN_INITIALIZING, SPN_DISARMED, SPN_ARMED, SPN_MAINTENANCE, SPN_SYS_ERROR
}; // end SumpPitNodeModes

class SumpPitNode
{
  public:
    /** Default constructor */
    SumpPitNode(Siren* _siren,
    Display* _display,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs,
    ShutoffValve* _shutoffValve);
    /** Default destructor */
    virtual ~SumpPitNode();
    virtual void setup();
    /**
      Update the system state. Not taking any actions.
      */
    virtual void update();

    /**
       Tells the currently selected mode.
       Value will be one of SumpPitNodeModes.
    */
    virtual int getMode();
    virtual void arm();
    virtual void disarm();
    virtual void maintenance();
    virtual void alarm();

    virtual int getAlarmReason();
  private: // methods
    void updateArmed();
    void alarmOff();
    bool isCritical(int reason);
    bool isTechnical(int reason);

  protected:
    Siren* siren;
    Display* display;
    SumpPitSensor* sensor;
    SumpPitInputs* inputs;
    ShutoffValve* shutoffValve;
  private: // variables
    int mode;
    int alarmReason;
};

#endif // SUMPPITNODE_H
