//#include "cellular_hal.h"
STARTUP(cellular_credentials_set("h2g2", "", "", NULL));

void setup() {
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
