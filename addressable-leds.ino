#define ADDR_LED_PIN 13

#define ADDR_LED_RANGE_1 0
#define ADDR_LED_RANGE_2 16
#define ADDR_LED_RANGE_3 32
#define ADDR_LED_RANGE_4 48
#define ADDR_LED_RANGE_5 64
#define ADDR_LED_RANGE_6 80

const int addr_led_numLeds = 100;

const unsigned long addr_led_refreshRate = 30;
const unsigned long addr_led_snakeRate = 20;
const unsigned long addr_led_blinkRate = 300;
unsigned long addr_led_lastRenderTime = 0;

bool addr_led_on = true;

CRGB leds[addr_led_numLeds];
CRGB tempLeds[addr_led_numLeds];

void addr_led_setup() {
  Serial.println("Setup addressable leds");
  FastLED.addLeds<WS2812, ADDR_LED_PIN, BRG>(leds, addr_led_numLeds);
  addr_led_setRange(ADDR_LED_RANGE_1, addr_led_numLeds, CRGB::Black);
}

void addr_led_reset() {
  addr_led_on = true;
  addr_led_setState(SIX);
}

void addr_led_setState(State state) {
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_refreshRate) {
    addr_led_setRange(ADDR_LED_RANGE_1, addr_led_numLeds, CRGB::Black);
    if (state >= 1) addr_led_setRange(ADDR_LED_RANGE_1, ADDR_LED_RANGE_2, CRGB::Red);
    if (state >= 2) addr_led_setRange(ADDR_LED_RANGE_2, ADDR_LED_RANGE_3, CRGB::Green);
    if (state >= 3) addr_led_setRange(ADDR_LED_RANGE_3, ADDR_LED_RANGE_4, CRGB::Blue);
    if (state >= 4) addr_led_setRange(ADDR_LED_RANGE_4, ADDR_LED_RANGE_5, CRGB::HotPink);
    if (state >= 5) addr_led_setRange(ADDR_LED_RANGE_5, ADDR_LED_RANGE_6, CRGB::Orange);
    if (state == 6) addr_led_setRange(ADDR_LED_RANGE_6, addr_led_numLeds, CRGB::Cyan);
    addr_led_render();
  }  
}

void addr_led_animateSnake() {
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_snakeRate) {
    for (int i = 0; i < addr_led_numLeds; i++) {
      if (i == 0) {
        addr_led_setColor(i, leds[addr_led_numLeds - 1]);
      } else {
        addr_led_setColor(i, leds[i - 1]);
      }
    }
    addr_led_render();
  }
}

void addr_led_animateBlink(State score) {
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_blinkRate) {
    if (addr_led_on) {
      addr_led_setState(score);
      addr_led_on = false;
    } else {
      addr_led_setRange(ADDR_LED_RANGE_1, addr_led_numLeds, CRGB::Black);
      addr_led_on = true;
    }
    addr_led_render();
  }
}

void addr_led_off() {
  addr_led_setRange(ADDR_LED_RANGE_1, addr_led_numLeds, CRGB::Black);
  addr_led_render();
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
  for (int i = 0; i < addr_led_numLeds; i++) {
    leds[i] = tempLeds[i];
  }
  FastLED.show();
  addr_led_lastRenderTime = millis();
}
