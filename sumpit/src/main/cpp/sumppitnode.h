#ifndef SUMPPITNODE_H
#define SUMPPITNODE_H

#include "display.h"
#include "siren.h"
#include "sumppitinputs.h"
#include "sumppitsensor.h"

// The main system modes
enum {
  SPN_INITIALIZING, SPN_DISARMED, SPN_ARMED, SPN_MAINTENANCE, SPN_SYS_ERROR
}; // end SumpPitNodeModes

#define SPN_ALARM_NO 0
#define SPN_ALARM_LEAK 1<<0
#define SPN_ALARM_PUMP_RPM_FAILURE 1<<1
#define SPN_ALARM_PUMP_VOLTAGE_FAILURE 1<<2
#define SPN_ALARM_HIGH_WATER 1<<3
#define SPN_ALARM_SYSTEM_ERROR 1<<4

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
    virtual void setup();
    /**
      Update the system state.
      Assuming sensor readings are done, or are instant.
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

  protected:
    Siren* siren;
    Display* display;
    SumpPitSensor* sensor;
    SumpPitInputs* inputs;
  private: // variables
    int mode;
    int alarmReason;
};

#endif // SUMPPITNODE_H
