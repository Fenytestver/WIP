#include "Particle.h"
#include "APNHelperRK.h"
STARTUP(cellular_credentials_set("h2g2", "", "", NULL));
SYSTEM_MODE(SEMI_AUTOMATIC);
//#include "cellular_hal.h"

const APNHelperAPN apns[2] = {
	{ "8901260", "fast.t-mobile.com"},
  { "8949320", "h2g2", "", ""}

};
APNHelper apnHelper(apns, sizeof(apns) / sizeof(apns[0]));


void setup() {
//  apnHelper.setCredentials();
  Particle.connect();
  // Set the keep-alive value for 3rd party SIM card here
	Particle.keepAlive(120);
  Serial.begin(115200);
  // your setup code
}

void loop() {
  delay(50);
  // your loop code
  Particle.process();
}
//
