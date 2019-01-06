#define PIN_OUT_1 3
#define PIN_OUT_2 4

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_OUT_1, OUTPUT);
  pinMode(PIN_OUT_2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delayMicroseconds(650);
  digitalWrite(PIN_OUT_1, HIGH);
  digitalWrite(PIN_OUT_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_OUT_1, LOW);
  digitalWrite(PIN_OUT_2, LOW);
}
