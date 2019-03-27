// This file intended to contain local configuration.
// Changes to this file should not be committed to the repo.

// Example:
// uncomment one of the lines below, keep the same lines commented in the
// application.ino file.
//
//#define PLATFORM PLATFORM_PHOTON
 #define PLATFORM PLATFORM_ELECTRON



// #define PIN_WATERLEVEL A0
#define PIN_LEAK_1 D2
#define PIN_LEAK_2 D3
// #define PIN_PUMP_VOLTAGE_1 A3
// #define PIN_PUMP_VOLTAGE_2 A4
#define PIN_SIREN D7
// #define PIN_LED_GREEN D5
// #define PIN_LED_YELLOW D4
// #define PIN_LED_RED D6
// #define PIN_PUMP_RPM_1 A2
// #define PIN_PUMP_RPM_2 A6

#define SIM_3RD_PARTY true

#define CLOUD_ENABLED true
#define SYNC_ENABLED true
#define SYNC_PERIOD_MIN 5000L
#define SYNC_PERIOD_MAX 60000L
#define SYNC_PERIOD_MAX_TECHNICAL 30000L
#define SYNC_PERIOD_MAX_CRITICAL 10000L
#define SPN_BUTTON_LONG_PRESS_TIME 1000
#define DISPLAY_I2C_ADDR 0x00

#include "Particle.h"
//STARTUP(cellular_credentials_set("h2g2", "", "", NULL));
#if PLATFORM == PLATFORM_ELECTRON
// #define PIN_BUTTON_1 B5
// #define PIN_BUTTON_2 B4
// #define PIN_BUTTON_3 B3
// #define PIN_BUZZER C0
#define PIN_FLOAT_SWITCH_1 C4
#endif
