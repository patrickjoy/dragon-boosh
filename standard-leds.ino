#define STD_LED_PIN 12

#define	STD_LED_VPLUS  0xFF3AC5
#define	STD_LED_VMINUS 0xFFBA45
#define	STD_LED_OFF 	  0xFF827D
#define	STD_LED_ON 	  0xFF02FD

#define	STD_LED_RED	  0xFF1AE5
#define	STD_LED_DARK_ORANGE	  0xFF2AD5
#define	STD_LED_ORANGE	  0xFF0AF5
#define	STD_LED_LIGHT_ORANGE	  0xFF38C7
#define	STD_LED_YELLOW	  0xFF18E7

#define	STD_LED_GREEN	  0xFF9A65
#define	STD_LED_SPRING_GREEN	  0xFFAA55
#define	STD_LED_SEA_FOAM	  0xFF8A75
#define	STD_LED_POOL	  0xFFB847
#define	STD_LED_CYAN	  0xFF9867 

#define	STD_LED_BLUE  	  0xFFA25D
#define	STD_LED_BLUE_PURPLE	  0xFF926D
#define	STD_LED_PURPLE	  0xFFB24D
#define	STD_LED_PINK_PURPLE	  0xFF7887
#define	STD_LED_MAGENTA	  0xFF58A7

#define	STD_LED_WHITE 	  0xFF22DD
#define	STD_LED_ROSE	  0xFF12ED
#define	STD_LED_LIGHT_PINK	  0xFF32CD
#define	STD_LED_BABY_BLUE	  0xFFF807
#define	STD_LED_ICE_BLUE	  0xFFD827

const long std_led_colors[7] = {
  STD_LED_RED, // READY
  STD_LED_BLUE, // ONE
  STD_LED_YELLOW, // TWO
  STD_LED_PURPLE, // THREE
  STD_LED_GREEN, // FOUR
  STD_LED_DARK_ORANGE, // FIVE
  STD_LED_WHITE, // SIX
};

const unsigned long std_led_refreshRate = 100;
const unsigned long std_led_rotationRate = 1000;
const unsigned long std_led_blinkRate = 300; // 150 minimum

unsigned long std_led_lastRenderTime = 0;

int std_led_currentState = -1;
bool std_led_on = true;

LedStripController std_led_channel;

void std_led_reset() {
  std_led_currentState = -1;
  std_led_on = true;
  std_led_changeColor(STD_LED_ON);
  delay(200);
  std_led_setState(READY);
}

void std_led_setup() {
  Serial.println("Setup standard leds");
  std_led_channel.begin(STD_LED_PIN);
  std_led_changeColor(STD_LED_ON);
  delay(200);
  std_led_setState(READY);
}

void std_led_animateRotation() {
  unsigned long currentTime = millis();
  if (currentTime - std_led_lastRenderTime >= std_led_rotationRate) {
    int numColors = sizeof(std_led_colors) / sizeof(std_led_colors[0]);
    int nextState = std_led_currentState + 1;
    if (nextState >= numColors) nextState = 1;
    std_led_setState(nextState);
  }
}

// Cannot turn on and change color at the same time.
void sts_led_animateBlink() {
  unsigned long currentTime = millis();
  if (currentTime - std_led_lastRenderTime >= std_led_blinkRate) {
    if (!std_led_on) {
      std_led_turn_on();
      std_led_on = true;
    } else {
      std_led_turn_off();
      std_led_on = false;
    }
  }
}

void std_led_turn_on() {
  if (!std_led_on) {
    std_led_changeColor(STD_LED_ON);
    std_led_on = true;
  }
}

void std_led_turn_off() {
  if (std_led_on) {
    std_led_changeColor(STD_LED_OFF);
    std_led_on = false;
  }
}

void std_led_changeColor(long color) {
  std_led_channel.writeCommand(color); // 71 millisecond operation
  std_led_lastRenderTime = millis();
}

void std_led_setState(int stateNum) {
  // escape if state is out of range
  if (stateNum < 0 || stateNum > 6) {
    return;
  }
  State state = static_cast<State>(stateNum);
  std_led_setState(state);
}

void std_led_setState(State state) {
  // no-op return
  if (std_led_currentState == state) {
    return;
  }

  // escape if state is out of range
  if (state < 0 || state > 6) {
    return;
  }

  std_led_changeColor(std_led_colors[state]);
  std_led_currentState = state;
}
