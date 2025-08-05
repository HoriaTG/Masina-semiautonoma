#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 caractere, 2 rânduri

RF24 radioTx(7, 8); // transmitator
const byte addressTx[6] = "00001";

RF24 radioRx(4, 5); // receptor
const byte addressRx[6] = "00002";

int joyX = A0;
int joyY = A1;

volatile bool asistentActiv = false;
volatile bool stareSchimbata = false;
volatile unsigned long ultimaApasare = 0;

const int joystickButtonPin = 2;
const unsigned long debounceInterval = 200;

int xValue;
int yValue;

int dir[3];

void setup() {
  Serial.begin(9600);
 
  radioTx.begin();
  radioTx.openWritingPipe(addressTx);
  radioTx.setPALevel(RF24_PA_MIN);
  radioTx.stopListening();

  radioRx.begin();
  radioRx.openReadingPipe(0, addressRx);
  radioRx.setPALevel(RF24_PA_MIN);
  radioRx.startListening();

  lcd.init();
  lcd.backlight();

  pinMode(joystickButtonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(joystickButtonPin), schimbaStareAsistent, FALLING);

  lcd.setCursor(0, 0);
  lcd.print("asistent OFF");
}

void loop() {

    if (stareSchimbata) {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (asistentActiv) {
      lcd.print("asistent ON");
    } else {
      lcd.print("asistent OFF");
    }
    stareSchimbata = false;
  }
  
  xValue = analogRead(joyX);  
  yValue = analogRead(joyY);
  xValue = map(xValue, 0, 1023, -512, 512);  
  yValue = map(yValue, 0, 1023, -512, 512);  

  dir[0] = xValue;
  dir[1] = yValue;
  dir[2] = asistentActiv;

  radioTx.write(&dir, sizeof(dir)); 

    if (radioRx.available()) {
    char mesajPrimit[32] = {0}; 
    radioRx.read(&mesajPrimit, sizeof(mesajPrimit));

    lcd.setCursor(0, 1); 
    lcd.print("                "); 
    lcd.setCursor(0, 1);

    char cod = mesajPrimit[0];

    Serial.println(cod);

    if (cod == ' ')
      lcd.print("             ");
    else if (cod == '1') {
      lcd.print("STOP");
    } else if (cod == '2') {
      lcd.print("CEDEAZA");
    } else if (cod == '3') {
      lcd.print("TRECERE");
    } else if (cod == '4') {
      lcd.print("PRIORITATE");
    } else {
      lcd.print(mesajPrimit); 
      Serial.println(mesajPrimit);
    }
  }

}

void schimbaStareAsistent() {
  unsigned long timpulCurent = millis();
  if (timpulCurent - ultimaApasare > debounceInterval) {
    asistentActiv = !asistentActiv;
    stareSchimbata = true;
    ultimaApasare = timpulCurent;
  }
}