#define BOOSH_PIN 2

const unsigned long boosh_booshDuration = 1000;
unsigned long boosh_lastBooshTime = 0;

bool boosh_booshed = false;

void boosh_setup() {
  Serial.println("Setup boosh");
  pinMode(BOOSH_PIN, OUTPUT);
}

void boosh_reset() {
  digitalWrite(BOOSH_PIN, LOW);
  boosh_booshed = false;
}

void boosh() {
  unsigned long currentTime = millis();
  if (!boosh_booshed) {
    Serial.println("boosh!!!");
    boosh_booshed = true;
    digitalWrite(BOOSH_PIN, HIGH);
    boosh_lastBooshTime = millis();
  } else if (currentTime - boosh_lastBooshTime >= boosh_booshDuration) {
    Serial.println("turning off boosh");
    digitalWrite(BOOSH_PIN, LOW);
  }
}