#ifdef DEBUG
//#include <iostream>
#endif // DEBUG

#ifndef SUMPPITNODE_H
#define SUMPPITNODE_H

#include "siren.h"
#include "sumppitinputs.h"
#include "sumppitsensor.h"
#include "spn_config.h"
#include "shutoffvalve.h"
#include "buzzer.h"
#include "localview.h"
#include "state.h"

using namespace spn;

class SumpPitNode
{
  public:
    /** Default constructor */
    SumpPitNode(Siren* _siren,
    Buzzer* _buzzer,
    LocalView* _localView,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs,
    ShutoffValve* _shutoffValve);
    // :( pls do not edit from outside)
    State state;

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

  protected:
    Siren* siren;
    LocalView* localView;
    SumpPitSensor* sensor;
    SumpPitInputs* inputs;
    ShutoffValve* shutoffValve;
    Buzzer* buzzer;

    void showState(State stateCopy);
  private: // variables

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
