const int motor1 = 13;
const int motor2 = 12;
// A fixed string of frequencies for now
String motorValues = "200000000";

// With 6 distance categories, we have 6 different vibration frequencies
int frequencies[] = {6, 5, 4, 3, 2, 1};
// corresponding to these intervals in milliseconds
int intervals[] = {167, 200, 250, 333, 500, 1000};

// Pins that have been tested to work with the vibration motors on the ESP32
int pins[] = {13, 12, 14, 15, 16, 17, 18, 19, 21};
long lastMillis = 0;

// See addToVibrationStack() below for an explanation
int vibrationStack[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int vibrationStackPointer = 0;

void setup()
{
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  // Add other motors later
  Serial.begin (115200);
}

int charToInt(char toConvert) {
  return toConvert - '0';
}

void loop() {
  int motorValuesArray[9];

  // Converts the string of vibration frequencies into an array
  for (int i = 0; i < 9; i++) {
    motorValuesArray[i] = charToInt(motorValues.charAt(i));
  }

  // Every second we check each interval
  for (int intervalIndex = 0; intervalIndex < 6; intervalIndex++) {
    long currMillis = millis();
    long diffMillis = currMillis - lastMillis;
    if (diffMillis >= 1000) {
      lastMillis = currMillis;
    }

    checkInterval(intervalIndex, diffMillis, motorValuesArray);

    for (int i = 0; i < 9; i++) {
      vibrationStack[i] = 0;
    }
    vibrationStackPointer = 0;
  }
}

// For a given interval, we check if there are any motors that are supposed to vibrate at that interval
void checkInterval(int intervalIndex, long startLoopMillis, int motorValuesArray[]) {
  // This could probably be replaced with 'startLoopMillis == intervals[intervalIndex]'
  // cuz we reset the millis counter every second
  if (startLoopMillis % intervals[intervalIndex] == 0) { 
    int currFreq = frequencies[intervalIndex];
    for (int motorIndex = 0; motorIndex < 9; motorIndex++) {
      // Modulo here because e.g. if we're checking a 1000ms interval, motors with both frequencies
      // 1 and 2 will need to vibrate.
      // Or if we're checking a 500 ms interval, motors with frequencies 2, 4 and 6 will need to vibrate.
      if ((motorValuesArray[motorIndex] != 0) && (motorValuesArray[motorIndex] % currFreq == 0)) {
        addToVibrationStack(motorIndex);
      }
    }
    vibrate();
  }
}

// There will be situations where we need to vibrate more than one motor at a time, so I implemented this
// very primitive stack. It's just an array where we keep track of the index of the next empty slot.
void addToVibrationStack(int motorIndex) {
  vibrationStack[vibrationStackPointer] = pins[motorIndex];
  vibrationStackPointer++;
}

// Vibrates every motor in the stack
void vibrate() {

  // Turns all of the required motors on
  for (int i = 0; i < vibrationStackPointer; i++) {
    int currMotor = vibrationStack[i];
    digitalWrite(currMotor, HIGH);
  }

  // Waits for 75 ms
  bool wait = true;
  long startLoopMillis = millis();
  do {
    long currMillis = millis();
    if (currMillis - startLoopMillis >= 75) {

      wait = false;

      // Turns all of them back off while resetting the stack
      for (int i = 0; i < vibrationStackPointer; i++) {
       int currMotor = vibrationStack[i];
       vibrationStack[i] = 0;
       digitalWrite(currMotor, LOW);
      }
    }
  } while (wait);

  // and the stack pointer
  vibrationStackPointer = 0;
}
