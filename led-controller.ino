// enum CelebrationStates {
//   CLIMB = 0,
//   WAIT = 1,
//   BLINK = 2,
// };

enum CelebrationStates {
  CLIMB = 0,
  WAIT = 1,
  BLINK_ON = 2,
  BLINK_OFF = 3,
};

const unsigned long led_ctl_blinkRate = 150; // TODO set to 300
unsigned long led_ctl_lastBlinkTime = 0;

CelebrationStates led_ctl_currentState = CLIMB;
CelebrationStates led_ctl_nextState = WAIT;

int led_ctl_climbTimes = 0;
int led_ctl_blinkTimes = 0;

bool led_ctl_blinkOn = true;
bool led_ctl_blinked = false;
bool led_ctl_skippedBlinkOff = false;
bool led_ctl_skippedBlinkOn = false;

void led_ctl_reset() {
  led_ctl_currentState = CLIMB;
  led_ctl_climbTimes = 0;
  led_ctl_blinkTimes = 0;
  led_ctl_blinkOn = true;
  led_ctl_blinked = false;
  led_ctl_skippedBlinkOff = false;
  led_ctl_skippedBlinkOn = false;
}

// void led_ctl_animateBlink(State score) {
//   unsigned long currentTime = millis();
//   if (currentTime - led_ctl_lastRenderTime >= led_ctl_blinkRate) {
//   if (!led_ctl_blinkOn) {
//       Serial.println("Turn leds on");

//       // addr_led_setState(score);
//       std_led_turn_on();
//       // Cannot turn on and change state at the same time
//       // std_led_setState(score);
//       led_ctl_blinkOn = true;
//     } else {
//       Serial.println("Turn leds off");
//       // addr_led_turn_off();
//       std_led_turn_off();
//       led_ctl_blinkOn = false;
//     }
//     led_ctl_lastRenderTime = millis();
//   }
// } 

/*
 coordinated blink
*/
// void led_ctl_celebrationSequence(State score) {
//   switch (led_ctl_celebrationState) {
//     case CLIMB: {
//       if (led_ctl_climbTimes >= 1) {
//         led_ctl_climbTimes = 0;
//         addr_led_reset_climbCount();
//         led_ctl_celebrationState = BLINK;
//       } else {
//         int climbCount = addr_led_animateClimb(score, 0);
//         if (climbCount != led_ctl_climbTimes) {
//           led_ctl_climbTimes = climbCount;
//         }
//       }
//       break;
//     }
//     case BLINK: {
//       if(led_ctl_blinkTimes >= 5) {
//         led_ctl_blinkTimes = 0;
//         led_ctl_celebrationState = CLIMB;
//         led_ctl_skippedBlinkOn = false;
//         led_ctl_skippedBlinkOff = false;
//       } else {
//         if (led_ctl_blinked != led_ctl_blinkOn) {
//           Serial.print("blinkOn: ");
//           Serial.println(led_ctl_blinkOn);
//           if (led_ctl_blinkTimes == 0 && !led_ctl_skippedBlinkOn) {
//             Serial.println("skip on");
//             if (led_ctl_blinkOn) led_ctl_skippedBlinkOn = true;
//           } else if (led_ctl_blinkTimes == 0 && led_ctl_skippedBlinkOn && !led_ctl_skippedBlinkOff) {
//             Serial.println("skip off");
//             if (!led_ctl_blinkOn) led_ctl_skippedBlinkOff = true;
//           } else {
//             if (led_ctl_blinkOn) led_ctl_blinkTimes++;
//             addr_led_blink(led_ctl_blinkOn, score);
//             Serial.print("blinkTimes: ");
//             Serial.println(led_ctl_blinkTimes);
//           }
//           led_ctl_blinked = led_ctl_blinkOn;
//         }
//       }
//       break;
//     }
//   }
// }

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

// void led_ctl_cycleBlinkState() {
//   unsigned long currentTime = millis();
//   if (currentTime - led_ctl_lastBlinkTime >= led_ctl_blinkRate) {
//     led_ctl_blinkOn = !led_ctl_blinkOn;
//     led_ctl_lastBlinkTime = millis();
//   }
// } 
