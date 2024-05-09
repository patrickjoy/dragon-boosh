#include <24KeysCodes.h>
#include <44KeysCodes.h>
#include <FastLED.h>
#include <LedStripController.h>
#include <SoftwareSerial.h>

#define BAUD_RATE 9600

enum State
{
  READY = 0,
  ONE = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  RESET = 7,
};

enum Response
{
  STOPPED = 0,
  PLAYING = 1,
  PAUSED = 2,
  NOT_AVAILABLE = 3,
  UNKNOWN = -1,
  UNRECOGNIZED = -2
};

const unsigned long stateTimeout = 5000;
const unsigned long resetTime = 4000;
unsigned long lastStateUpdate = 0;

State currentState = READY;
State score = READY;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Begin");

  mp3_setup();
  sensor_setup();
  addr_led_setup();
  std_led_setup();
  boosh_setup();

  addr_led_setState(SIX); // This sets the full pattern to cycle
  // mp3_playRandomSong(); // play first song
}

void loop() {
  // currentState = RESET;
  // currentScore = SIX;
  // Serial.println("Hello");
  // Serial.print("Current state: ");
  // Serial.println(currentState);
  // Serial.print("Score: ");
  // Serial.println(score);
  switch (currentState) {
    case READY: { // loop
      addr_led_animateSnake();
      std_led_animateRotation(); 
      // bool playing = mp3_isPlaying(); // check music status
      // if (!playing) {
      //   mp3_playNextSong(); // play next song
      // }
      State state = sensor_checkPins(currentState); // check hall effects
      // State state = sensor_test(currentState);
      checkForUpdate(state);
      break;
    }
    case ONE:
    case TWO:
    case THREE:
    case FOUR:
    case FIVE: { // hall effects 1-5
      addr_led_setState(currentState); // light up addressable leds based on state
      std_led_animateRotation(); 
      State state =  sensor_checkPins(currentState); // check hall effects
      // State state = sensor_test(currentState);
      checkForUpdate(state);
      break;
    }
    case SIX: { // hall effect 6
      score = currentState;
      std_led_animateRotation(); 
      addr_led_setState(currentState); // light up addressable leds based on state
      changeState(RESET);
      break;
    }
    case RESET: {
      if (score == SIX) {
        boosh();
      }
      // play sound effects
      mp3_playCelebration(score);
      std_led_setState(score); // light up standard leds based on score
      addr_led_animateBlink(score); // blink addressable leds
      // blink standard leds
      resetWhenReady();
      break;
    }
  }
  // incrementState();
}

void resetWhenReady() {
  unsigned long currentTime = millis();
  if (currentTime - lastStateUpdate >= resetTime) {
    boosh_reset(); // reset boosh
    addr_led_reset(); // reset leds
    mp3_reset(); // reset mp3
    score = READY; // reset score
    changeState(READY); // reset state
  }
}

void checkForUpdate(State state) {
  if (state != currentState) {
    score = state;
    changeState(state);
  } else if (isTimedout(state)) {
    changeState(RESET);
  }
}

void changeState(State state) {
  currentState = state;
  lastStateUpdate = millis();
}

bool isTimedout(State state) {
  unsigned long currentTime = millis();
  if ((currentTime - lastStateUpdate >= stateTimeout) && (state != READY)) {
    return true;
  }
  return false;
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
    else if (currentState == SIX) changeState(ONE);
  }
}