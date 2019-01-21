Sensor node
===========

```
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
    // Returns the current device id value; see setDeviceId function
		"deviceId": "int32",
    // Returns the current system mode;
    // 0,1,2...:
    // SPN_INITIALIZING, SPN_DISARMED, SPN_ARMED, SPN_MAINTENANCE, SPN_SYS_ERROR
		"mode": "int32",
    // Pump 1 RPM
		"rpm1": "int32",
    // Pump 2 RPM
		"rpm2": "int32",
    // If pump 1 is on
		"pump1On": "int32",
    // If pump 2 is on
		"pump2On": "int32",
    // The current status string
		"status": "string",
    // Pump 1 uptime in milliseconds
		"p1Uptime": "int32",
    // Pump 2 uptime in milliseconds
		"p2Uptime": "int32",
    // Water level in inches
		"levelIn": "int32",
    // Water level percentage
		"levelP": "int32",
    // If leak is detected
		"leak": "bool",
    // If shutoff valve is on
		"shutoffValve": "bool",
    // If the float switch is triggered
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
    // Sets the value (analogRead result) where 'water high' is
    // integer parameter; returns the saved value
    "setWaterHigh",
    // Sets the value (inches) to return at 'water low'
    // integer parameter; returns the saved value
    "setWaterLowIn",
    // Sets the value (inches) to return at 'water high'
    // integer parameter; returns the saved value
    "setWaterHighIn",
    // Required for calibration. Set "[low-high distance],[high-critical distance]" values as
    // 2 comma separated int. Calculated high percentage is returned when values are accepted.
    "setWaterLevels",
    // Saves current settings to EEPROM and reboots the system.
    // no parameters; does not return any value, request will fail.
    "reboot",
    // Clears the saved calibration values and reboots.
    // no parameters; returns 0;
    "clearCalib",
    // Snoozes the alarm; integer parameter: time to sleep in milliseconds;
    // returns the remaining snooze time; no or invalid parameter will not change the
    // remaining time.
    "snooze"
  ]
}
```
