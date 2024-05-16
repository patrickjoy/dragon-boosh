enum CelebrationStates {
  CLIMB = 0,
  WAIT = 1,
  BLINK_ON = 2,
  BLINK_OFF = 3,
};

const unsigned long led_ctl_blinkRate = 150;
unsigned long led_ctl_lastBlinkTime = 0;

CelebrationStates led_ctl_currentState = CLIMB;
CelebrationStates led_ctl_nextState = WAIT;

int led_ctl_climbTimes = 0;
int led_ctl_blinkTimes = 0;

void led_ctl_reset() {
  led_ctl_currentState = CLIMB;
  led_ctl_climbTimes = 0;
  led_ctl_blinkTimes = 0;
}

void led_ctl_celebrationSequence(State score) {
  switch (led_ctl_currentState) {
    case CLIMB: {
      // Serial.println("CLIMB");
      if (led_ctl_climbTimes >= 1) {
        led_ctl_climbTimes = 0;
        addr_led_reset_climbCount();
        led_ctl_currentState = WAIT;
        led_ctl_nextState = BLINK_OFF;
        led_ctl_lastBlinkTime = millis();
      } else {
        int climbCount = addr_led_animateClimb(score, led_ctl_blinkRate);
        if (climbCount != led_ctl_climbTimes) {
          led_ctl_climbTimes = climbCount;
        }
      }
      break;
    }
    case WAIT: {
      // Serial.println("WAIT");
      unsigned long currentTime = millis();
      if (currentTime - led_ctl_lastBlinkTime >= led_ctl_blinkRate) {
        led_ctl_currentState = led_ctl_nextState;
        switch(led_ctl_nextState) {
          case CLIMB: {
            led_ctl_nextState = BLINK_OFF;
            break;
          }
          case BLINK_ON: {
            led_ctl_nextState = BLINK_OFF;
            break;
          }
          case BLINK_OFF: {
            led_ctl_nextState = BLINK_ON;
            break;
          }
        }
      }
      break;
    }
    case BLINK_ON: {
      // Serial.println("BLINK_ON");
      led_ctl_blinkTimes++;
      led_ctl_currentState = WAIT;
      led_ctl_nextState = BLINK_OFF;
      addr_led_blink(false, score);
      led_ctl_lastBlinkTime = millis();
      break;
    }
    case BLINK_OFF: {
      // Serial.println("BLINK_OFF");
      if (led_ctl_blinkTimes >= 5) {
        led_ctl_nextState = CLIMB;
        led_ctl_blinkTimes = 0;
      } else {
        led_ctl_nextState = BLINK_ON;
      }
      led_ctl_currentState = WAIT;
      addr_led_blink(true, score);
      led_ctl_lastBlinkTime = millis();
      break;
    }
  }
}
