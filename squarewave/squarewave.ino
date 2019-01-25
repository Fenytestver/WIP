// connect rpm1: D4, rpm2: D3
#define PIN_OUT_1 4
#define PIN_OUT_2 3
// uncommend these to use an analog signal to produce rpm
#define PIN_IN_1 A3
#define PIN_IN_2 A2

// enter RPM here
#define RPM1 1700
#define RPM2 1000

long p1half = 0;
long p2half = 0;
long p1time = 0;
long p2time = 0;
bool p1 = false;
bool p2 = false;

int rpm1, rpm2;
int cycle1;
int cycle2;
unsigned long now;
unsigned long lastRead = 0;
unsigned long nowms;

void setup() {
  pinMode(PIN_OUT_1, OUTPUT);
  pinMode(PIN_OUT_2, OUTPUT);
  Serial.begin(115200);
  
#ifdef PIN_IN_1
  pinMode(PIN_IN_1, INPUT);
  pinMode(PIN_IN_2, INPUT);
  rpm1 = analogRead(PIN_IN_1);
  p1half = rpmToMicros(rpm1);
  rpm2 = analogRead(PIN_IN_2);
  p2half = rpmToMicros(rpm2);
#else
  rpm1 = RPM1;
  rpm2 = RPM2;
  p1half = rpmToMicros(RPM1);
  p2half = rpmToMicros(RPM2);
#endif
  
}


void loop() {
  now = millis();
  //nowms = millis();
  cycle1 = now - p1time;
  if (rpm1 > 0 && cycle1 > p1half) {
    p1 = !p1;
    digitalWrite(PIN_OUT_1, p1 ? LOW : HIGH);
    p1time = now - (cycle1 - p1half);
  }
  
  cycle2 = now - p2time;
  if (rpm2 > 0 && cycle2 > p2half) {
    p2 = !p2;
    digitalWrite(PIN_OUT_2, p2 ? LOW : HIGH);
    p2time = now - (cycle2 - p2half);
  }


#ifdef PIN_IN_1
  if (now - lastRead > 500) {
    rpm1 = map(analogRead(PIN_IN_1), 5, 1024, 0, 3000);
    if (rpm1 < 10) rpm1 = 0;
    p1half = rpmToMicros(rpm1);
    rpm2 = map(analogRead(PIN_IN_2), 5, 1024, 0, 3000);
    if (rpm2 < 10) rpm2 = 0;
    p2half = rpmToMicros(rpm2);
    lastRead = now;
  }
#endif
}

int rpmToMicros(int rpm) {
  return round(60.0 * 1000.0 / ((double)rpm)) / 2 /* two times per Hz; up, down */;  
}

