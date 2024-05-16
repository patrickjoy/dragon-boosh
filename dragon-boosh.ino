#include <24KeysCodes.h>
#include <44KeysCodes.h>
#include <FastLED.h>
#include <LedStripController.h>
#include <SoftwareSerial.h>

#define BAUD_RATE 9600

enum State {
  READY = 0,
  ONE = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  RESET = 7,
};

enum Response {
  STOPPED = 0,
  PLAYING = 1,
  PAUSED = 2,
  NOT_AVAILABLE = 3,
  UNKNOWN = -1,
  UNRECOGNIZED = -2
};

const unsigned long stateTimeout = 500; // TODO (set to 500)
const unsigned long resetTime = 28000; // TODO (songs are 28 seconds long)
unsigned long lastStateUpdate = 0;

State currentState = READY;
State score = READY;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Begin");

  int randomPin = analogRead(A0);
  randomSeed(randomPin * 100); // set random seed

  mp3_setup();
  sensor_setup();
  addr_led_setup();
  new_led_setup();
  boosh_setup();

  mp3_playRandomSong();
}

void loop() {
  // unsigned long startTime = millis();
  // Serial.print("Current state: ");
  // Serial.println(currentState);
  // Serial.print("Score: ");
  // Serial.println(score);
  // score = SIX;
  // currentState = RESET;
  switch (currentState) {
    case READY: { // loop
      bool playing = mp3_checkPlaying(); // check music status
      if (!playing) {
        mp3_playRandomSong(); // 8 milliseconds
      }
      // State state = sensor_checkPins(currentState); // check hall effects
      State state = sensor_test(currentState);
      checkForUpdate(state);
      break;
    }
    case ONE:
    case TWO:
    case THREE:
    case FOUR:
    case FIVE: { // hall effects 1-5
      // State state = sensor_checkPins(currentState); // check hall effects
      State state = sensor_test(currentState);
      checkForUpdate(state);
      break;
    }
    case SIX: { // hall effect 6
      score = currentState;
      changeState(RESET);
      break;
    }
    case RESET: {
      if (score == SIX) {
        // boosh_longBoosh();
        // boosh_shortBoosh();
        boosh();
      }
      mp3_playCelebration(score); // play sound effects
      new_led_setState(score);
      led_ctl_celebrationSequence(score);
      
      resetWhenReady();
      break;
    }
  }
  // incrementState();
  // unsigned long totalLength = millis() - startTime;
  // if (totalLength > 5) {
    // Serial.print("Loop time: ");
    // Serial.println(totalLength);
    // Serial.println(startTime);
  // }
}

void resetWhenReady() {
  unsigned long currentTime = millis();
  if (currentTime - lastStateUpdate >= resetTime) {
    boosh_reset(); // reset boosh
    // delay(100);
    led_ctl_reset();
    new_led_reset();
    addr_led_reset(); // reset addressable leds
    mp3_reset(); // reset mp3
    score = READY; // reset score
    changeState(READY); // reset state
  }
}

void checkForUpdate(State state) {
  if (state == currentState) {
    checkTimeout(state);
  } else if (state > currentState) {
    score = state;
    changeState(state);
  } else if (state < currentState) {
    changeState(RESET);
  }
}

void checkTimeout(State state) {
  unsigned long currentTime = millis();
  if ((currentTime - lastStateUpdate >= stateTimeout) && (state != READY)) {
    changeState(RESET);
  }
}

void changeState(State state) {
  currentState = state;
  lastStateUpdate = millis();
}

void incrementState() {
  unsigned long rate = 1000;
  unsigned long currentTime = millis();
  if (currentTime - lastStateUpdate > rate) {
    if (currentState == READY) changeState(ONE);
    else if (currentState == ONE) changeState(TWO);
    else if (currentState == TWO) changeState(THREE);
    else if (currentState == THREE) changeState(FOUR);
    else if (currentState == FOUR) changeState(FIVE);
    else if (currentState == FIVE) changeState(SIX);
    else if (currentState == SIX) changeState(RESET);
  }
}
