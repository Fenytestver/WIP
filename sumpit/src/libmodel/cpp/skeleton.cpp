// include guard
#ifndef skeleton_cpp
#define skeleton_cpp
// TODO(marcell): particle.io std lib?

////
// Generic interfaces:
////

class Updateable {
  public:
    /* Called in every loop */
    virtual void update();
};

////
// Generic outputs:
////

/** Just a led */
class Led {
  public:
    virtual void setState(bool on);
};
/** The big loud siren */
class Siren {
  public:
    /** Turn on the alarm. */
    void on();
    /** Turn off the alarm. */
    void off();
};

/**
   Buzzer is same as a Siren,
   with an extra option for a short beep.
*/
class Buzzer : Siren {
  public:
    /** beeps once. */
    virtual void beep();
};

/** A simple display (i2c, ..). */
class Display {
  public:
    virtual void displayMessage(char*);
    virtual void clear();
};
////
// Button
////
class OnButtonPressListener {
  public:
    virtual void onPress();
};
class Button {
  public:
    void setOnButtonPressListener(OnButtonPressListener* onPressListener) {
      _onPressListener = onPressListener;
    }
  protected:
    OnButtonPressListener* _onPressListener = 0;
};

////
// Sensors
////
class RpmSensor {
  public:
    virtual int getRpm();
};

class VoltageSensor {
  public:
    // TODO: is this on/off or 0-12v?
    /** returns the measured voltage */
    virtual float getVoltage();
};

class WaterLevelSensor {
  public:
    /** @return the water level in percentage. */
    virtual short measureLevel();
};

class LeakSensor {
  public:
    /** gives true if there is a leak. */
    virtual bool isLeaking();
};
////
// Complex classes
////
class Pump {
  public:
    /** gives the on/off state of the pump, as we requested. */
    virtual bool isTurnedOn();
    /** gives the on/off state of the pump based on it's voltage. */
    virtual bool isVoltageDetected();
    /** @return returns the RPM of the pump. */
    virtual int getRpm();
  protected:
    RpmSensor* rmpSensor;
    VoltageSensor* voltageSensor;
};

/** Sensors on the Sump pit sensor */
class SumpPitSensor {
  public:
    /** Call at setup phase, once */
    virtual void setup();
  protected:
    WaterLevelSensor* waterLevelSensors;
    Pump* pump;
    LeakSensor* LeakSensors;
};


class SumpPitInputs {
  protected:
    Button* disarmButton;
    Button* maintenanceButton;
    Button* armResetButton;
};

////
// SumpPitNodeModes: states for the critial node
////

// The main system modes
enum {
  INITIALIZING, DISARMED, ARMED, SYS_ERROR
}; // end SumpPitNodeModes

class SumpPitNode {
  public:
    /**
       Tells the currently selected mode.
       Value will be one of SumpPitNodeModes.
    */
    virtual int getMode();

  protected:
    Siren* siren;
    Display* display;
    SumpPitSensor* sensor;
    SumpPitInputs* inputs;
};

// Actuator/remote-controller & related
// The main remote node modes
enum {
  REM_INITIALIZING, REM_STANDBY, REM_ALARM, REM_SYS_ERROR
}; // end RemoteNodeModes
class Snoozable {
  public:
    virtual void snooze();
};
class SnoozableSiren : Snoozable, Siren {
  public:
    virtual void snooze() {
      snoozedAt = 999; // FIXME: get current time
    }
  private:
    int snoozedAt = 0;
};
class RemoteNodeInputs {
  protected:
    Button* snoozeButton;
};
class RemoteNodeOutputs {
  protected:
    Led* onLed;
    Led* maintenanceLed;
    Buzzer* buzzer;
    SnoozableSiren* siren;
};
class RemoteNode {
  protected:
    int mode;
    RemoteNodeInputs* inputs;
    RemoteNodeOutputs* outputs;
};

#endif // skeleton_cpp
