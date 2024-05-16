#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

void new_led_reset() {
    new_led_setState(READY);
}

void new_led_setup() {
  Serial.println("Setup new leds");
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  new_led_setState(READY);
}

void new_led_setState(State state) {
  switch(state) {
    case READY: {
      new_led_writeRGB(255, 0, 0); // RED
      break;
    }
    case ONE: {
      new_led_writeRGB(0, 0, 255); // Blue
      break;
    }
    case TWO: {
      new_led_writeRGB(255, 255, 0); // Yellow
      break;
    }
    case THREE: {
      new_led_writeRGB(128, 0, 128); // Purple
      break;
    }
    case FOUR: {
      new_led_writeRGB(0, 128, 0); // Green
      break;
    }
    case FIVE: {
      new_led_writeRGB(255, 69, 0); // RedOrange
      break;
    }
    case SIX: {
      new_led_writeRGB(255, 255, 255); // White
      break;
    }
  }
}

void new_led_writeRGB(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
