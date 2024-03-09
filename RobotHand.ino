#include <Servo.h>

#define numOfValsRec 5
#define digitsPerValRec 1

int valsRec[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec + 1;  // $00000
int counter = 0;
bool counterStart = false;
String receivedString = "";

Servo servoThumb;
Servo servoIndex;
Servo servoMiddle;
Servo servoRing;
Servo servoPinky;

void setup() {
  Serial.begin(9600);

  servoThumb.attach(7);
  servoIndex.attach(9);
  servoMiddle.attach(11);
  servoRing.attach(8);
  servoPinky.attach(10);

  receivedString.reserve(20); // Reserve enough space to avoid dynamic allocation
}

void receiveData() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '$') {
      counterStart = true;
      receivedString = ""; // Reset the string when a new message starts
      counter = 0; // Reset counter for each new message
    }
    if (counterStart) {
      receivedString += c; // Append received character
      counter++;
      if (counter >= stringLength) {
        for (int i = 0; i < numOfValsRec; i++) {
          int num = (i * digitsPerValRec) + 1;
          valsRec[i] = receivedString.substring(num, num + digitsPerValRec).toInt();
          Serial.print(valsRec[i]);
        }
        receivedString = ""; // Reset for the next message
        counter = 0;
        counterStart = false;
      }
    }
  }
}

void loop() {
  receiveData();
  servoThumb.write(valsRec[0] = 1 ? 180 : 0);
  servoIndex.write(valsRec[1] == 1 ? 180 : 0);
  servoMiddle.write(valsRec[2] == 1 ? 180 : 0);
  servoRing.write(valsRec[3] == 1 ? 180 : 0);
  servoPinky.write(valsRec[4] == 1 ? 180 : 0);
}
