#define BOOSH_PIN 2

const unsigned long boosh_longDuration = 1000;
const unsigned long boosh_shortDuration = 333;
const unsigned long boosh_pauseDuration = 333;

unsigned long boosh_lastTurnedOn = 0;
unsigned long boosh_lastTurnedOff = 0;

int boosh_count = 0;
int boosh_state = 0;

bool boosh_booshed = false;

void boosh_reset() {
  boosh_off();
  boosh_count = 0;
  boosh_state = 0;
  boosh_booshed = false;
}

void boosh_setup() {
  Serial.println("Setup boosh");
  pinMode(BOOSH_PIN, OUTPUT);
  boosh_off();
}

void boosh() {
  if (boosh_state == 0) boosh_state = random(1, 3);

  switch(boosh_state) {
    case 1: {
      boosh_longBurst();
      break;
    }
    case 2: {
      boosh_shortBursts();
      break;
    }
  }
}

void boosh_longBurst() {
  bool booshBooshing = boosh_isBooshing(boosh_longDuration);
  if (!boosh_booshed) {
    // Serial.println("big boosh");
    boosh_booshed = true;
    boosh_on();
  } else if (boosh_booshed && !booshBooshing) { 
    boosh_off();
  }
}

void boosh_shortBursts() {
  bool booshBooshing = boosh_isBooshing(boosh_shortDuration);
  bool booshPaused = boosh_isPaused();

  // escape if boosh sequence is done
  if (boosh_count >= 3 && !booshBooshing) {
    boosh_off();
    return;
  }

  if (!boosh_booshed && !booshPaused) {
    // Serial.println("small boosh");
    boosh_booshed = true;
    boosh_count++;
    boosh_on();
  } else if (boosh_booshed && !booshBooshing) { 
    boosh_booshed = false;
    boosh_off();
  }
}

bool boosh_isBooshing(unsigned long booshDuration) {
  unsigned long currentTime = millis();
  if (currentTime - boosh_lastTurnedOn < booshDuration) {
    return true;
  }
  return false;
}

bool boosh_isPaused() {
  unsigned long currentTime = millis();
  if (currentTime - boosh_lastTurnedOff < boosh_pauseDuration) {
    return true;
  }
  return false;
}

void boosh_on() {
  // Serial.println("turn boosh on");
  boosh_lastTurnedOn = millis();
  digitalWrite(BOOSH_PIN, HIGH);
}

void boosh_off() {
  // Serial.println("turn boosh off");
  boosh_lastTurnedOff = millis();
  digitalWrite(BOOSH_PIN, LOW);
}
