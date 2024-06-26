#define ADDR_LED_PIN 13


const int start_led = 6; // (6th copper connector)
const int addr_led_numLeds = 68 + start_led + 3;

int addr_led_ranges[7] {
  0 + start_led,   // blank
  12 + start_led,  // blue
  26 + start_led + 1 ,  // yellow
  38 + start_led + 2,  // purple
  48 + start_led + 2,  // green
  57 + start_led + 2, // orange
  68 + start_led + 3, // white
};

CRGB addr_led_colors[7] = {
  CRGB::Black, // Off
  CRGB::Blue, // Rat
  CRGB::Yellow, // Rooster
  CRGB::Purple, // Horse
  CRGB::Green, // Ox
  CRGB::OrangeRed, // Tiger
  CRGB::White, // Dragon
};

const unsigned long addr_led_refreshRate = 10;
const unsigned long addr_led_snakeRate = 20;
const unsigned long addr_led_climbRate = 50;
const unsigned long addr_led_climbPause = 1000;

unsigned long addr_led_lastRenderTime = 0;

bool addr_led_climbStarted = false;

int addr_led_nextLedToRender = 0;
int addr_led_numClimbs = 0;

CRGB displayLeds[addr_led_numLeds];
CRGB stagingLeds[addr_led_numLeds];
CRGB goalLeds[addr_led_numLeds];

void addr_led_reset() {
  addr_led_climbStarted = false;
  addr_led_nextLedToRender = 0;
  addr_led_numClimbs = 0;
  addr_led_setState(SIX, goalLeds);
  addr_led_setState(SIX, stagingLeds);
}

void addr_led_reset_climbCount() {
  addr_led_numClimbs = 0;
}

void addr_led_setup() {
  Serial.println("Setup addressable leds");
  FastLED.addLeds<WS2812, ADDR_LED_PIN, BRG>(displayLeds, addr_led_numLeds);
  addr_led_setState(SIX, goalLeds);
  addr_led_setState(SIX, stagingLeds);
}

void addr_led_setState(State state) {
  addr_led_setState(state, stagingLeds);
}

void addr_led_setState(State state, CRGB leds[]) {
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_refreshRate) {
    addr_led_setRange(0, addr_led_numLeds, addr_led_colors[0], leds);
    if (state >= 1) addr_led_setRange(addr_led_ranges[0], addr_led_ranges[1], addr_led_colors[1], leds);
    if (state >= 2) addr_led_setRange(addr_led_ranges[1], addr_led_ranges[2], addr_led_colors[2], leds);
    if (state >= 3) addr_led_setRange(addr_led_ranges[2], addr_led_ranges[3], addr_led_colors[3], leds);
    if (state >= 4) addr_led_setRange(addr_led_ranges[3], addr_led_ranges[4], addr_led_colors[4], leds);
    if (state >= 5) addr_led_setRange(addr_led_ranges[4], addr_led_ranges[5], addr_led_colors[5], leds);
    if (state == 6) addr_led_setRange(addr_led_ranges[5], addr_led_ranges[6], addr_led_colors[6], leds);
    if (leds == stagingLeds) {
      addr_led_render();
    }
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

int addr_led_animateClimb(State state) {
  return addr_led_animateClimb(state, addr_led_climbPause);
}

int addr_led_animateClimb(State state, unsigned long pause) {
  int limit = addr_led_ranges[state];
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_climbRate) {
    if (!addr_led_climbStarted) {
      addr_led_setRange(0, addr_led_numLeds, addr_led_colors[0]);
      addr_led_render();
      addr_led_climbStarted = true;
    } else if (addr_led_nextLedToRender < limit) {
      // update multiple leds at a time
      for (int i = 0; i < 3; i++) {
        if (addr_led_nextLedToRender >= limit) break;
        addr_led_setColor(addr_led_nextLedToRender, goalLeds[addr_led_nextLedToRender]);
        addr_led_nextLedToRender++;
      }
      addr_led_render();
    } else if (currentTime - addr_led_lastRenderTime >= pause){
      addr_led_numClimbs++;
      addr_led_nextLedToRender = 0;
      addr_led_climbStarted = false;
    }
  }
  return addr_led_numClimbs;
}

void addr_led_blink(bool blink, State score) {
  if(blink) {
    addr_led_turn_off();
  } else {
    addr_led_setState(score);
  }
}


void addr_led_turn_off() {
  addr_led_off(stagingLeds);
}

void addr_led_off(CRGB leds[]) {
  addr_led_setRange(0, addr_led_numLeds, addr_led_colors[0], leds);
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
  unsigned long currentTime = millis();
  if (currentTime - addr_led_lastRenderTime >= addr_led_refreshRate) {
    for (int i = 0; i < addr_led_numLeds; i++) {
      displayLeds[i] = leds[i];
    }
    FastLED.show();
    addr_led_lastRenderTime = millis();
  }
}

// void addr_test() {
//   while(true) {
//     delay(250);
//     addr_led_turn_off();
//     delay(250);

//     for(int i = 0; i < addr_led_numLeds; i++) {
//       displayLeds[i] = CRGB(CRGB::Yellow);
//       delay(20);
//       FastLED.show();
//       delay(250);
//     }
//   }
// }
