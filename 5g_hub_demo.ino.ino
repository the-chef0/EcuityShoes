#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

const int motor1 = 13;
const int motor2 = 12;
int pins[] = {13, 12, 14, 15, 16, 17, 18, 19, 21};
long lastMillis = 0;
String motorValues = "00";

void setup()
{
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  Serial.begin (115200);
  SerialBT.begin();
}

int charToInt(char toConvert) {
  return toConvert - '0';
}

void loop() {
  if (SerialBT.available()) {
    motorValues = SerialBT.readStringUntil('\n');

    Serial.println(motorValues);
  }

  int motorValuesArray[2];
  for (int i = 0; i < 2; i++) {
      motorValuesArray[i] = charToInt(motorValues.charAt(i));
  }

  long currMillis = millis();
    long diffMillis = currMillis - lastMillis;
    if (diffMillis >= 1000) {
      lastMillis = currMillis;
    }

    if  (diffMillis % 1000 == 0) {
      if (motorValuesArray[0] != 0) {
        Serial.println(String(motorValuesArray[0]));
        vibrate(pins[0]);
      }
      if (motorValuesArray[1] != 0) {
        vibrate(pins[1]);
      }
    }
  
    if (diffMillis % 500 == 0) {
      if (motorValuesArray[0] == 2) {
        vibrate(pins[0]);
      }
      if (motorValuesArray[1] == 2) {
        vibrate(pins[1]);
      }
    }

  Serial.println(motorValues);
  
}

void vibrate(int pin) {
  digitalWrite(pin, HIGH);
  
  bool wait = true;
  long startLoopMillis = millis();
  do {
    long currMillis = millis();
    if (currMillis - startLoopMillis >= 75) {

      wait = false;
      digitalWrite(pin, LOW);
    }
  } while (wait);
}
