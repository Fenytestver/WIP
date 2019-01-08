// connect rpm1: D4, rpm2: D3
#define PIN_OUT_1 4
#define PIN_OUT_2 3

// enter RPM here
#define RPM1 500
#define RPM2 800

long p1half = 0;
long p2half = 0;
long p1time = 0;
long p2time = 0;
bool p1 = false;
bool p2 = false;

void setup() {
  pinMode(PIN_OUT_1, OUTPUT);
  pinMode(PIN_OUT_2, OUTPUT);
  p1half = round(1000000.0 / ((double)RPM1) / 2.0);
  p2half = round(1000000.0 / ((double)RPM2) / 2.0);
}

void loop() {
  long now = micros();
  if (now - p1time > p1half) {
    p1 = !p1;
    digitalWrite(PIN_OUT_1, p1 ? LOW : HIGH);
    p1time = now;
  }
  if (now - p2time > p2half) {
    p2 = !p2;
    digitalWrite(PIN_OUT_2, p2 ? LOW : HIGH);
    p2time = now;
  }
}
