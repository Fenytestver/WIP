#ifdef DEBUG
//#include <iostream>
#endif // DEBUG

#ifndef SPNCONFIG_H
#define SPNCONFIG_H

// Number of pumps
#define SPN_PUMP_COUNT 2
// give a little time after a turn on to spin up the pump
#define SPN_PUMP_SPINUP_TIME 10000L
// Pump uptime that triggers technical alert. No alert below this.
#define SPN_PUMP_CYCLE_MAX_LENGTH_TECHNICAL 30000
// Pump uptime that triggers critical alert. Technical alert below this.
#define SPN_PUMP_CYCLE_MAX_LENGTH_CRITICAL 40000
// The usual RPM of the pump motor
#define SPN_PUMP_STD_RPM 400
// Technical alert above this. Deviation to STD_RPM (unit:RPM +/-).
// Normal below this.
#define SPN_PUMP_RPM_DEVI_TECHNICAL 40
// The instability quotient for rpm. The margin between turning on/off.
#define SPN_PUMP_RPM_DEVI_VARIANCE 5
// Technical alert above this. Deviation to STD_RPM (unit:RPM +/-).
// Technical alert below this.
#define SPN_PUMP_RPM_DEVI_CRITICAL 70
// Voltage threshold
#define SPN_PUMP_LOW_VOLTAGE_THRESHOLD 10.0

// Water levels
// Pump turns off when reached
#define SPN_WATER_LOW 10
// Pump should turn on at this level
#define SPN_WATER_HIGH 20
// Critical water level.
#define SPN_WATER_CRITICAL 80
#define SPN_WATER_VARIANCE 10
// Magic, do not touch.
// Alert levels
#define SPN_ALERT_NO_ALERT 0
#define SPN_ALERT_TECHNICAL 1
#define SPN_ALERT_CRITICAL 2

// Node related
#define SPN_ALARM_NO_ALARM (0)
#define SPN_ALARM_LEAK (1<<0)
#define SPN_ALARM_PUMP_RPM_TECHNICAL (1<<1)
#define SPN_ALARM_PUMP_RPM_CRITICAL (1<<2)
#define SPN_ALARM_PUMP_VOLTAGE_CRITICAL (1<<3)
#define SPN_ALARM_PUMP_CYCLE_TECHNICAL (1<<4)
#define SPN_ALARM_PUMP_CYCLE_CRITICAL (1<<5)
#define SPN_ALARM_WATER_CRITICAL (1<<6)
#define SPN_ALARM_WATER_LOW (1<<7)
#define SPN_ALARM_SYSTEM_ERROR (1<<8)
// all technical alerts
#define SPN_ALERT_ALL_TECHNICAL \
  (SPN_ALARM_PUMP_RPM_TECHNICAL \
   | SPN_ALARM_PUMP_CYCLE_TECHNICAL \
   | SPN_ALARM_WATER_LOW)

// all critical alerts
#define SPN_ALERT_ALL_CRITICAL \
  (SPN_ALARM_LEAK | SPN_ALARM_PUMP_RPM_CRITICAL \
   | SPN_ALARM_PUMP_VOLTAGE_CRITICAL \
   | SPN_ALARM_PUMP_CYCLE_CRITICAL \
   | SPN_ALARM_WATER_CRITICAL)

namespace spn {
  int abso(int a);
  void sb(int& flag, int mask, bool condition);
}

#endif // SPNCONFIG_H
