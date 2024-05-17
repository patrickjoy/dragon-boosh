#define SENSOR_THRESHOLD 100

const int sensor_count = 6;
const int sensorPins[sensor_count] {
  A0,
  A1,
  A2,
  A3,
  A4,
  A5
};

void sensor_setup() {
  Serial.println("Setup hall effect sensors");
  for (int pin : sensorPins) {
    pinMode(pin, INPUT);
  }
}

State sensor_checkPins(State state) {
  for (int i = sensor_count - 1; i >= 0; i--) {
    int pin = sensorPins[i];
    int value = analogRead(pin);
    // Serial.print("Pin ");
    // Serial.println(i);
    // Serial.println(": ");
    // Serial.println(value);
    if (value < SENSOR_THRESHOLD) {
      // Serial.print("Magnet detected on pin: ");
      // Serial.println(i);
      return static_cast<State>(i + 1);
    }
  }
  return state;
}

const unsigned long testRate = 400;
unsigned long lastTest = 0;
State sensor_test(State state) {
  unsigned long currentTime = millis();
  if(currentTime - lastTest >= testRate) {
    int pin = sensorPins[0];
    int value = analogRead(pin);
    // Serial.print("Value: ");
    // Serial.println(value);

    if (value < SENSOR_THRESHOLD) {
      Serial.println("Magnet detected");
      State tmpState = static_cast<State>(state + 1);
      Serial.print("Sent to state: ");
      Serial.println(tmpState);
      lastTest = millis();
      return tmpState;
    }
  }
  return state;
}
