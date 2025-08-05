#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo myServo;

const int motor1pin1 = 22;
const int motor1pin2 = 24;
const int motor2pin1 = 26;
const int motor2pin2 = 28;
const int motor3pin1 = 32;
const int motor3pin2 = 30;
const int motor4pin1 = 34;
const int motor4pin2 = 36;

const int leftSensorPin = 9;
const int rightSensorPin = 10;

int servoPin = 44;
int servoPos = 60;

int dir[3];

RF24 radioRx(7, 8); // receptor
const byte addressRx[6] = "00001";

RF24 radioTx(4, 5); // transmitator
const byte addressTx[6] = "00002";

bool alertLinie = false;
bool alertStop = false;

bool vedeStop = false;
bool stopADisparut = false;
bool utilizatorS_aOprit = false;
bool stopInVizor = false;

unsigned long lastStopDetected = 0;
const unsigned long stopDisparutDelay = 2000; 

char c;

const char msg[] = "STOP linie IR";
const char msgStop[] = "STOP asist";

void setup() {
  Serial.begin(9600);

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);  
  pinMode(motor3pin1, OUTPUT);
  pinMode(motor3pin2, OUTPUT);
  pinMode(motor4pin1, OUTPUT);
  pinMode(motor4pin2, OUTPUT);

  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(servoPos);  

  radioRx.begin();
  radioRx.openReadingPipe(0, addressRx);
  radioRx.setPALevel(RF24_PA_MIN);
  radioRx.startListening();

  radioTx.begin();
  radioTx.openWritingPipe(addressTx);
  radioTx.setPALevel(RF24_PA_MIN);
  radioTx.stopListening();
}

void loop() {
  int leftSensorValue = digitalRead(leftSensorPin);
  int rightSensorValue = digitalRead(rightSensorPin);

  if (radioRx.available()) {
    radioRx.read(&dir, sizeof(dir));
    int asistentActiv = dir[2];

    if (!asistentActiv) {
      alertLinie = false;
      alertStop = false;
      vedeStop = false;
      stopADisparut = false;
      utilizatorS_aOprit = false;
      stopInVizor = false;
    }

    if (Serial.available()) {
      c = Serial.read();
      if (c == '1') {
        vedeStop = true;
        stopInVizor = true;
        lastStopDetected = millis();
        stopADisparut = false;
      }
    }

    if (millis() - lastStopDetected > stopDisparutDelay && vedeStop) {
      vedeStop = false;
      stopInVizor = false;

      if (!utilizatorS_aOprit) {
        stopADisparut = true;  // asistentul intervine
      }

      utilizatorS_aOprit = false;  // reset 
    }

    bool linieDetectata = (leftSensorValue == LOW || rightSensorValue == LOW);

    if (linieDetectata && asistentActiv) {
      opresteMotoarele();
      alertLinie = true;
    }
    else if (stopADisparut && asistentActiv) {
      opresteMotoarele();
      alertStop = true;
    }
    else {
      alertLinie = false;
      alertStop = false;

      // motoare
      if (dir[0] < -400) {  // inainte
        digitalWrite(motor1pin1, HIGH);
        digitalWrite(motor1pin2, LOW);
        digitalWrite(motor2pin1, HIGH);
        digitalWrite(motor2pin2, LOW);
        digitalWrite(motor3pin1, LOW);
        digitalWrite(motor3pin2, HIGH);
        digitalWrite(motor4pin1, LOW);
        digitalWrite(motor4pin2, HIGH);
      } else if (dir[0] > 400) {  // inapoi
        digitalWrite(motor1pin1, LOW);
        digitalWrite(motor1pin2, HIGH);
        digitalWrite(motor2pin1, LOW);
        digitalWrite(motor2pin2, HIGH);
        digitalWrite(motor3pin1, HIGH);
        digitalWrite(motor3pin2, LOW);
        digitalWrite(motor4pin1, HIGH);
        digitalWrite(motor4pin2, LOW);
      } else {
        opresteMotoarele();

        if (stopInVizor) {
          utilizatorS_aOprit = true;
        }
      }

      // Control servo
      servoPos = map(dir[1], -512, 512, 20, 100);
      servoPos = constrain(servoPos, 20, 100);
      myServo.write(servoPos);
    }
  }

  // Mesaje catre telecomanda
  if (alertLinie) {
    radioTx.write(&msg, sizeof(msg));
  } else if (alertStop) {
    radioTx.write(&msgStop, sizeof(msgStop));
  } else {
    radioTx.write(&c, sizeof(c));
  }
}

void opresteMotoarele() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);
  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, LOW);
}
