#define ADDR_LED_PIN 13

const int addr_led_numLeds = 100;

int addr_led_ranges[6] {
  16, // range 1
  32, // range 2
  48, // range 3
  64, // range 4
  80, // range 5
  100, // range 6
};

const unsigned long addr_led_refreshRate = 30;
const unsigned long addr_led_blinkRate = 300;
const unsigned long addr_led_snakeRate = 20;
const unsigned long addr_led_climbRate = 40;
const unsigned long addr_led_climbPause = 2000;

unsigned long addr_led_lastRenderTime = 0;

bool addr_led_on = true;

int addr_led_nextLedToRender = 0;

enum addr_led_state {
  ADDR_LED_READY = 0,
  ADDR_LED_ONE = 1,
  ADDR_LED_TWO = 2,
  ADDR_LED_THREE = 3,
  ADDR_LED_FOUR = 4,
  ADDR_LED_FIVE = 5,
  ADDR_LED_SIX = 6
};

CRGB displayLeds[addr_led_numLeds];
CRGB stagingLeds[addr_led_numLeds];
CRGB goalLeds[addr_led_numLeds];

void addr_led_setup() {
  Serial.println("Setup addressable leds");
  FastLED.addLeds<WS2812, ADDR_LED_PIN, BRG>(displayLeds, addr_led_numLeds);
  addr_led_setRange(0, addr_led_numLeds, CRGB::Black, stagingLeds);
  addr_led_setState(SIX, goalLeds);

  // testing
  // addr_led_render();
  // delay(200);
  while (true) {
    addr_led_animateClimb(SIX);
  }
}

void addr_led_reset() {
  addr_led_setState(SIX);
  addr_led_on = true;
}

void addr_led_setState(State state) {
  addr_led_setState(state, stagingLeds);
  addr_led_render(); 
}

void addr_led_setState(State state, CRGB leds[]) {
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_refreshRate) {
    addr_led_setRange(0, addr_led_numLeds, CRGB::Black, leds);
    if (state >= 1) addr_led_setRange(0, addr_led_ranges[0], CRGB::Red, leds);
    if (state >= 2) addr_led_setRange(addr_led_ranges[0], addr_led_ranges[1], CRGB::Green, leds);
    if (state >= 3) addr_led_setRange(addr_led_ranges[1], addr_led_ranges[2], CRGB::Blue, leds);
    if (state >= 4) addr_led_setRange(addr_led_ranges[2], addr_led_ranges[3], CRGB::HotPink, leds);
    if (state >= 5) addr_led_setRange(addr_led_ranges[3], addr_led_ranges[4], CRGB::Orange, leds);
    if (state == 6) addr_led_setRange(addr_led_ranges[4], addr_led_ranges[5], CRGB::Cyan, leds);
  }  
}

void addr_led_animateSnake() {
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_snakeRate) {
    for (int i = 0; i < addr_led_numLeds; i++) {
      if (i == 0) {
        addr_led_setColor(i, displayLeds[addr_led_numLeds - 1]);
      } else {
        addr_led_setColor(i, displayLeds[i - 1]);
      }
    }
    addr_led_render();
  }
}

void addr_led_animateClimb(State state) {
  int limit = addr_led_ranges[state-1];
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_climbRate) {
    if (addr_led_nextLedToRender >= limit) {
      if (currentTime - addr_led_lastRenderTime >= addr_led_climbPause) {
        addr_led_setRange(0, addr_led_numLeds, CRGB::Black);
        addr_led_nextLedToRender = 0;
        addr_led_render();
      }
    } else {
      addr_led_setColor(addr_led_nextLedToRender, goalLeds[addr_led_nextLedToRender]);
      addr_led_nextLedToRender++;
      addr_led_render();
    }
  }
}

void addr_led_animateBlink(State score) {
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_blinkRate) {
    if (addr_led_on) {
      addr_led_setState(score);
      addr_led_on = false;
    } else {
      addr_led_setRange(0, addr_led_numLeds, CRGB::Black);
      addr_led_on = true;
    }
    addr_led_render();
  }
}

void addr_led_off() {
  addr_led_off(stagingLeds);
}

void addr_led_off(CRGB leds[]) {
  addr_led_setRange(0, addr_led_numLeds, CRGB::Black, leds);
  addr_led_render();
}

void addr_led_setRange(int start, int end, CRGB color) {
  addr_led_setRange(start, end, color, stagingLeds);
}

void addr_led_setRange(int start, int end, CRGB color, CRGB leds[]) {
  for (int i = start; i < end; i++) {
    addr_led_setColor(i, color, leds);
  }
}

void addr_led_setColor(int led, CRGB color) {
  addr_led_setColor(led, color, stagingLeds);
}

void addr_led_setColor(int led, CRGB color, CRGB leds[]) {
  leds[led] = CRGB(color);
}

void addr_led_render() {
  addr_led_render(stagingLeds);
}

void addr_led_render(CRGB leds[]) {
  for (int i = 0; i < addr_led_numLeds; i++) {
    displayLeds[i] = leds[i];
  }
  FastLED.show();
  addr_led_lastRenderTime = millis();
}
