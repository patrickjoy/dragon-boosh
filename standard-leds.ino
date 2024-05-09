#define STD_LED_PIN 12

// #define	STD_LED_VPlus      0xFF3AC5
// #define	STD_LED_VMinus 0xFFBA45
// #define	STD_LED_LED_ON 	  0xFF827D
// #define	STD_LED_LED_OFF 	  0xFF02FD

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

const unsigned long std_led_rotationRate = 1000;
unsigned long std_led_lastChangeTime = 0;
int std_led_currentColor = 0;

const long std_led_colors[6] = {
  STD_LED_RED,
  STD_LED_GREEN,
  STD_LED_BLUE,
  STD_LED_ROSE,
  STD_LED_LIGHT_ORANGE,
  STD_LED_ICE_BLUE,
};

LedStripController std_led_channel;

void std_led_setup() {
  Serial.println("Setup standard leds");
  std_led_channel.begin(STD_LED_PIN);
  // TODO turn on
}

void std_led_animateRotation() {
  unsigned long currentTime = millis();
  if (currentTime - std_led_lastChangeTime >= std_led_rotationRate) {
    std_led_currentColor++;
    int numColors = sizeof(std_led_colors) / sizeof(std_led_colors[0]);
    if (std_led_currentColor >= numColors) std_led_currentColor = 0;
    std_led_changeColor(std_led_colors[std_led_currentColor]);
  }
}

void std_led_changeColor(long color) {
  std_led_channel.writeCommand(color);
  std_led_lastChangeTime = millis();
}

void std_led_setState(State state) {
  if (state == 1) std_led_changeColor(STD_LED_RED);
  if (state == 2) std_led_changeColor(STD_LED_GREEN);
  if (state == 3) std_led_changeColor(STD_LED_BLUE);
  if (state == 4) std_led_changeColor(STD_LED_ROSE);
  if (state == 5) std_led_changeColor(STD_LED_LIGHT_ORANGE);
  if (state == 6) std_led_changeColor(STD_LED_ICE_BLUE);
}