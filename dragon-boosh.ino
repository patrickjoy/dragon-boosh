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

// Rates
const unsigned long stateTimeout = 500; // TODO (set to 500)
const unsigned long resetTime = 10000; // TODO (songs are 28 seconds long)
unsigned long lastStateUpdate = 0;

State currentState = READY;
State score = READY;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Begin");

  int pin_0 = analogRead(0);
  randomSeed(pin_0 * 100); // set random seed

  mp3_setup();
  sensor_setup();
  addr_led_setup();
  std_led_setup();
  boosh_setup();

  mp3_playRandomSong(); // play first song

  /* 
    TODO remove testing
  */
  // delay(500);
  // while(true) {
    // score = SIX;

    /*
      test colors
    */
    // std_led_changeColor(0xFF22DD);

    /* 
      state tests
    */
    // addr_led_setState(THREE);
    // std_led_setState(score);
 

    /*
      blink tests
    */
    // addr_led_animateBlink(FIVE);
    // sts_led_animateBlink();
    // led_ctl_animateBlink(score);

    /* 
      rotation tests
    */
    // std_led_animateRotation(); // 71 milli seconds every second

    /*
      climb tests
    */
    // Serial.println(addr_led_animateClimb(THREE));
    // delay(500);
  //   led_ctl_cycleBlinkState();
  //   led_ctl_celebrationSequence(FIVE);
  // }
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
      // addr_led_animateSnake(); // 4 milliseconds
      // std_led_animateRotation(); // 71 milli seconds every second
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
      // addr_led_setState(currentState);
      // std_led_animateRotation(); 
      // State state =  sensor_checkPins(currentState); // check hall effects
      State state = sensor_test(currentState);
      checkForUpdate(state);
      break;
    }
    case SIX: { // hall effect 6
      // Serial.println("boosh");
      score = currentState;
      // std_led_animateRotation(); 
      // addr_led_setState(currentState); // light up addressable leds based on state
      changeState(RESET);
      break;
    }
    case RESET: {
      if (score == SIX) {
        // boosh_longBurst();
        boosh_shortBursts();
        // boosh();
      }
      mp3_playCelebration(score); // play sound effects
      // std_led_setState(score);
      // sts_led_animateBlink();
      led_ctl_celebrationSequence(score);
      
      resetWhenReady();
      break;
    }
  }

  // redundant safety feature in case signal drops
  if (!boosh_isBooshing()) {
    boosh_off();
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
    delay(100);
    addr_led_reset(); // reset addressable leds
    std_led_reset(); // reset standard leds
    led_ctl_reset();
    mp3_reset(); // reset mp3
    score = READY; // reset score
    changeState(READY); // reset state
  }
}

// OLD CODE - EXPECTS STATE TO ONLY INCREAE
// void checkForUpdate(State state) {
//   if (state != currentState) {
//     score = state;
//     changeState(state);
//   } else if (checkTimeout(state)) {
//     changeState(RESET);
//   }
// }

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
    // Serial.println("Timedout");
    changeState(RESET);
  }
}

void changeState(State state) {
  // Serial.print("Changing to state: ");
  // Serial.println(state);
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