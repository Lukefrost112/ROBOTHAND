#include <Servo.h>
#include <SoftwareSerial.h>

// Define Servos
Servo Thumb, Index, Middle, Ring, Little;

// Initialize SoftwareSerial for Bluetooth communication
SoftwareSerial mySerial(10, 11);

// Positions for each finger
int ThumbPos, IndexPos, MiddlePos, RingPos, LittlePos;

// Speed settings
int fingerSpeed = 20, HandSpeed = 20;

// LED and Button pins
int BLED = 13;
int RLED = 12;
int SWITCH = 9;

// Variables for serial communication
#define numOfValsRec 5
#define digitsPerValRec 1
int valsRec[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec + 1;  // $00000
int counter = 0;
bool counterStart = false;
String receivedString = "";

void setup() {
  // Initialize Serial and SoftwareSerial
  Serial.begin(9600);
  mySerial.begin(9600);

  // Initialize LED and Button pins
  pinMode(BLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);

  // Attach servos
  Thumb.attach(2);
  Index.attach(3);
  Middle.attach(4);
  Ring.attach(5);
  Little.attach(6);

  // Initialize servos to starting position
  Thumb.write(45);
  ThumbPos = 45;
  Index.write(45);
  IndexPos = 45;
  Middle.write(45);
  MiddlePos = 45;
  Ring.write(45);
  RingPos = 45;
  Little.write(45);
  LittlePos = 45;

  // Turn off LEDs
  digitalWrite(RLED, LOW);
  digitalWrite(BLED, LOW);

  // Reserve space for receivedString to avoid dynamic allocation
  receivedString.reserve(20);
}

void loop() {
  int ProgSwitch;
  ProgSwitch = digitalRead(SWITCH);
  if (ProgSwitch == 0) {
    BLUETOOTH();
    digitalWrite(RLED, LOW);
    digitalWrite(BLED, HIGH);
  } else if (ProgSwitch == 1) {
    SERIAL_COMMUNICATION();
    digitalWrite(RLED, HIGH);
    digitalWrite(BLED, LOW);
  }
}


void BLUETOOTH() {
  if (mySerial.available()) {
    char command = mySerial.read();
    int targetPos = 0;
    switch (command) {
      case 'T':
        targetPos = (ThumbPos == 45) ? 150 : 45;
        moveFinger(Thumb, ThumbPos, targetPos, fingerSpeed);
        break;
      case 'I':
        targetPos = (IndexPos == 45) ? 150 : 45;
        moveFinger(Index, IndexPos, targetPos, fingerSpeed);
        break;
      case 'M':
        targetPos = (MiddlePos == 45) ? 150 : 45;
        moveFinger(Middle, MiddlePos, targetPos, fingerSpeed);
        break;
      case 'R':
        targetPos = (RingPos == 45) ? 150 : 45;
        moveFinger(Ring, RingPos, targetPos, fingerSpeed);
        break;
      case 'L':
        targetPos = (LittlePos == 45) ? 150 : 45;
        moveFinger(Little, LittlePos, targetPos, fingerSpeed);
        break;
      default:
        // Handle unexpected commands or implement additional features here
        break;
    }
  }
}

void SERIAL_COMMUNICATION() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '$') {
      counterStart = true;
      receivedString = "";  // Reset the string when a new message starts
      int counter = 0;      // Reset counter for each new message
    }
    if (counterStart) {
      receivedString += c;  // Append received character
      counter++;
      if (counter >= stringLength) {
        for (int i = 0; i < numOfValsRec; i++) {
          int num = (i * digitsPerValRec) + 1;
          valsRec[i] = receivedString.substring(num, num + digitsPerValRec).toInt();
        }
        // Apply the received values to the servo positions
        Thumb.write(valsRec[0] == 1 ? 150 : 45);
        ThumbPos = valsRec[0] == 1 ? 150 : 45;
        Index.write(valsRec[1] == 1 ? 150 : 45);
        IndexPos = valsRec[1] == 1 ? 150 : 45;
        Middle.write(valsRec[2] == 1 ? 150 : 45);
        MiddlePos = valsRec[2] == 1 ? 150 : 45;
        Ring.write(valsRec[3] == 1 ? 150 : 45);
        RingPos = valsRec[3] == 1 ? 150 : 45;
        Little.write(valsRec[4] == 1 ? 150 : 45);
        LittlePos = valsRec[4] == 1 ? 150 : 45;

        receivedString = "";  // Reset for the next message
        counter = 0;
        counterStart = false;
      }
    }
  }
}

void moveFinger(Servo& finger, int& fingerPos, int targetPos, int speed) {
  int direction = (fingerPos < targetPos) ? 1 : -1;
  while (fingerPos != targetPos) {
    fingerPos += direction;
    finger.write(fingerPos);
    delay(speed);
  }
}
