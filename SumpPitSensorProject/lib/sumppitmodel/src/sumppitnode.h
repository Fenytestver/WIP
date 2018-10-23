#ifdef DEBUG
//#include <iostream>
#endif // DEBUG

#ifndef SUMPPITNODE_H
#define SUMPPITNODE_H

#include "display.h"
#include "siren.h"
#include "sumppitinputs.h"
#include "sumppitsensor.h"
#include "spn_config.h"
#include "shutoffvalve.h"
#include "buzzer.h"
#include "localview.h"
#include "state.h"

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
    Buzzer* _buzzer,
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
    virtual State* update();

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
    Buzzer* buzzer;

    void showState(State stateCopy);
  private: // variables
    State state;

    class OnArmPress : public OnButtonPressListener {
      public:
        OnArmPress(SumpPitNode* _node) {node = _node;}
        void onPress() {
          node->disarm();
        }
      private:
        SumpPitNode* node;
    } *armPressListener;

    class OnDisarmPress : public OnButtonPressListener {
      public:
        OnDisarmPress(SumpPitNode* _node) {node = _node;}
        void onPress() {
          node->disarm();
        }
      private:
        SumpPitNode* node;
    } *disarmPressListener;

    class OnMaintenancePress : public OnButtonPressListener {
      public:
        OnMaintenancePress(SumpPitNode* _node) {node = _node;}
        void onPress() {
          node->maintenance();
        }
      private:
        SumpPitNode* node;
    } *mainrenancePressListener;

    class OnAnyPress : public OnButtonPressListener {
      public:
        OnAnyPress(Buzzer* _buzzer) {buzzer=_buzzer;}
        void onPress() {
          buzzer->beep();
        }
      private:
        Buzzer* buzzer;
    } *beepPressListener;

};

#endif // SUMPPITNODE_H
