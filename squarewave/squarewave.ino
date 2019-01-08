// connect rpm1: D4, rpm2: D3
#define PIN_OUT_1 4
#define PIN_OUT_2 3

// enter RPM here
#define RPM1 1700
#define RPM2 1000

long p1half = 0;
long p2half = 0;
long p1time = 0;
long p2time = 0;
bool p1 = false;
bool p2 = false;

void setup() {
  pinMode(PIN_OUT_1, OUTPUT);
  pinMode(PIN_OUT_2, OUTPUT);
  p1half = rpmToMicros(RPM1);
  p2half = rpmToMicros(RPM2);
}

int cycle1;
int cycle2;
long now;

void loop() {
  now = micros();
  cycle1 = now - p1time;
  if (cycle1 > p1half) {
    p1 = !p1;
    digitalWrite(PIN_OUT_1, p1 ? LOW : HIGH);
    p1time = now - (cycle1 - p1half);
  }
  cycle2 = now - p2time;
  if (cycle2 > p2half) {
    p2 = !p2;
    digitalWrite(PIN_OUT_2, p2 ? LOW : HIGH);
    p2time = now - (cycle2 - p2half);
  }
}

int rpmToMicros(int rpm) {
  return round(1000000.0 / ((double)rpm) / 2.0);
}

