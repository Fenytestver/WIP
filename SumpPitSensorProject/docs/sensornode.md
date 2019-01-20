Sensor node
===========

"variables": {
    // Returns the current waterLow value; see setWaterLow function
		"waterLow": "int32",
    // Returns the current waterHigh value; see setWaterHigh function
		"waterHigh": "int32",
    // Returns the current waterLowIn value; see setWaterLowIn function
		"waterLowIn": "int32",
    // Returns the current waterHighIn value; see setWaterHighIn function
		"waterHighIn": "int32",
    // Returns the current waterCriPer value; see setWaterCriPer function
		"waterCriPer": "int32",
    // Returns the current waterHigh value; see setWaterHigh function
		"deviceId": "int32",
    // Returns the current system mode;
    // 0,1,2...:
    // SPN_INITIALIZING, SPN_DISARMED, SPN_ARMED, SPN_MAINTENANCE, SPN_SYS_ERROR
		"mode": "int32",

		"rpm1": "int32",
		"rpm2": "int32",
		"pump1On": "int32",
		"pump2On": "int32",
		"status": "string",
		"p1Uptime": "int32",
		"p2Uptime": "int32",
		"levelIn": "int32",
		"levelP": "int32",
		"leak": "bool",
		"shutoffValve": "bool",
		"floatSwitch": "bool"
	},
	"functions": [
    // Arms the system; no parameters; returns 0;
    "arm",
    // Disarms the system; no parameters; returns 0;
    "disarm",
    // Starts maintenance mode; no parameters; returns 0;
    "maintenance",
    // Restarts the LCD screen; no parameters; returns 0;
    "lcdInit",
    // Requests an immediate update; no parameters; returns 0;
    "update",
    // Requests the contents of the LCD; no parameters; returns 0;
    // After calling this method, contents of the screen will be
    // publshed to "lcdtext" topic.
    "getLcd",
    // Sets and saves the device id; integer parameter; returns the saved id;
    "setDeviceId",
    // Returns the raw analogRead value of the water level sensor.
    "getRawLevel",
    // Sets the value (analogRead result) where 'water low' is
    // integer parameter; returns the saved value
    "setWaterLow",
    // Sets the value (analogRead result) where 'high low' is
    // integer parameter; returns the saved value
    "setWaterHigh",
    // Sets the value (inches) to return at 'water low'
    // integer parameter; returns the saved value
    "setWaterLowIn",
    // Sets the value (inches) to return at 'water high'
    // integer parameter; returns the saved value
    "setWaterHighIn",
    // Sets the value (percent) where the 'critical water level' will be placed
    // integer parameter; returns the saved value.
    // See SPN_WATER_LOW, SPN_WATER_HIGH, SPN_WATER_CRITICAL values is spn_config.h
    "setWaterCriPer",
    // Saves current settings to EEPROM and reboots the system.
    // no parameters; does not return any value, request will fail.
    "reboot",
    // Clears the saved calibration values and reboots.
    // no parameters; returns 0;
    "clearCalib"
  ]
}
