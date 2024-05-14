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

// unsigned long resetRate = 5000;
// unsigned long startTime = 0;
// int results[sensor_count] {
//   0,
//   0,
//   0,
//   0,
//   0,
//   0
// };

// measure last pin
void sensor_setup() {
  Serial.println("Setup hall effect sensors");
  for (int pin : sensorPins) {
    pinMode(pin, INPUT); // setup pins
  }
}

// OLD CODE - ONLY CHECKS PINS ABOVE CURRENT PIN
// State sensor_checkPins(State state) {
//   int pinIndex = 0;
//   if (state != READY) pinIndex = state - 1; // READY would otherwise be -1 and crash the program.

//   // check all states above current state
//   for (int i = sensor_count - 1; i >= pinIndex; i--) {
//     // Serial.print("check pin: ");
//     // Serial.println(i);
//     int pin = sensorPins[i];
//     int value = analogRead(pin);
//     // Serial.println(value); // TODO remove
//     if (value < SENSOR_THRESHOLD) {
//       // Serial.print("Magnet detected on pin: "); // TODO remove
//       // Serial.println(i);
//       return static_cast<State>(i + 1);
//     }
//   }
//   return state;
// }

State sensor_checkPins(State state) {
  // printTest();
  int pinIndex = 0;
  if (state != READY) pinIndex = state - 1; // READY would otherwise be -1 and crash the program.

  // check all pins
  for (int i = sensor_count - 1; i >= 0; i--) {
    // Serial.print("check pin: ");
    // Serial.println(i);
    int pin = sensorPins[i];
    int value = analogRead(pin);
    // Serial.println(value); // TODO remove
    if (value < SENSOR_THRESHOLD) {
      // Serial.print("Magnet detected on pin: "); // TODO remove
      // Serial.println(i);
      // results[i]++;
      return static_cast<State>(i + 1);
    }
  }
  return state;
}

// void printTest() {
//   unsigned long currentTime = millis();
//   unsigned long totalTime = currentTime - startTime;
//   // Serial.print("Total time: ");
//   // Serial.println(totalTime);
//   if (totalTime >= resetRate) {
//     // Serial.print("Current Time: ");
//     // Serial.println(currentTime);
//     // Serial.print("Start time: ");
//     // Serial.println(startTime);
//     for (int i = 0; i < sensor_count; i++) {
//       Serial.print("pin ");
//       Serial.print(i);
//       Serial.print(" count: ");
//       Serial.println(results[i]);
//       results[i] = 0;
//     }
//     startTime = millis();
//   }
// }


const unsigned long testRate = 250;
unsigned long lastTest = 0;
State sensor_test(State state) {
  unsigned long currentTime = millis();
  if(currentTime - lastTest >= testRate) {
    int pin = sensorPins[0];
    int value = analogRead(pin);
    // Serial.print("Value: ");
    // Serial.println(value);

    if (value < SENSOR_THRESHOLD) {
      Serial.println("Magnet detected"); // TODO remove
      State tmpState = static_cast<State>(state + 1);
      Serial.print("Sent to state: ");
      Serial.println(tmpState);
      lastTest = millis();
      return tmpState;
    }
  }
  return state;
}