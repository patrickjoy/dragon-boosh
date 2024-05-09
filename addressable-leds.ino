#define ADDR_LED_PIN 13

#define ADDR_LED_RANGE_1 0
#define ADDR_LED_RANGE_2 16
#define ADDR_LED_RANGE_3 32
#define ADDR_LED_RANGE_4 48
#define ADDR_LED_RANGE_5 64
#define ADDR_LED_RANGE_6 80

const int NUM_LEDS = 100;

const unsigned long refreshRate = 30;
const unsigned long snakeRate = 20;
const unsigned long blinkRate = 300;

unsigned long lastRenderTime = 0;

bool addr_led_on = true;

CRGB leds[NUM_LEDS];
CRGB tempLeds[NUM_LEDS];

void addr_led_setup() {
  Serial.println("Setup addressable leds");
  FastLED.addLeds<WS2812, ADDR_LED_PIN, BRG>(leds, NUM_LEDS);
  addr_led_setRange(ADDR_LED_RANGE_1, NUM_LEDS, CRGB::Black); // set to black to start
}

void addr_led_reset() {
  addr_led_on = true;
  addr_led_setState(SIX);
}

void addr_led_setState(State state) {
  unsigned long currentTime = millis();
  if (currentTime - lastRenderTime >= refreshRate) {
    addr_led_setRange(ADDR_LED_RANGE_1, NUM_LEDS, CRGB::Black);
    if (state >= 1) addr_led_setRange(ADDR_LED_RANGE_1, ADDR_LED_RANGE_2, CRGB::Red);
    if (state >= 2) addr_led_setRange(ADDR_LED_RANGE_2, ADDR_LED_RANGE_3, CRGB::Green);
    if (state >= 3) addr_led_setRange(ADDR_LED_RANGE_3, ADDR_LED_RANGE_4, CRGB::Blue);
    if (state >= 4) addr_led_setRange(ADDR_LED_RANGE_4, ADDR_LED_RANGE_5, CRGB::HotPink);
    if (state >= 5) addr_led_setRange(ADDR_LED_RANGE_5, ADDR_LED_RANGE_6, CRGB::Orange);
    if (state == 6) addr_led_setRange(ADDR_LED_RANGE_6, NUM_LEDS, CRGB::Cyan);
    addr_led_render();
  }  
}

void addr_led_animateSnake() {
  unsigned long currentTime = millis();
  if (currentTime - lastRenderTime >= snakeRate) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i == 0) {
        addr_led_setColor(i, leds[NUM_LEDS - 1]);
      } else {
        addr_led_setColor(i, leds[i - 1]);
      }
    }
    addr_led_render();
  }
}

void addr_led_animateBlink(State score) {
  unsigned long currentTime = millis();
  if (currentTime - lastRenderTime >= blinkRate) {
    if (addr_led_on) {
      addr_led_setState(score);
      addr_led_on = false;
    } else {
      addr_led_setRange(ADDR_LED_RANGE_1, NUM_LEDS, CRGB::Black); // set to black to start
      addr_led_on = true;
    }
    addr_led_render();
  }
}

void addr_led_setRange(int start, int end, CRGB color) {
  for (int i = start; i < end; i++) {
    addr_led_setColor(i, color);
  }
}

void addr_led_setColor(int led, CRGB color) {
  tempLeds[led] = CRGB(color);
}

void addr_led_render() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = tempLeds[i];
  }
  FastLED.show();
  lastRenderTime = millis();
}
