SYSTEM_MODE(MANUAL);

void setup() {
  for (int i=0; i<256; ++i) {
    EEPROM.put(i, 0x0);
  }
  Particle.connect();
}
void loop() {
}
