#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// TX to Slave: pin 10 (TX), dummy RX at 11
SoftwareSerial slaveSerial(10, 11); 

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int startButton = 7;
const int trigPin = 5;
const int echoPin = 6;

const int redLED = 8;
const int yellowLED = 9;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Washing Machine");
  lcd.setCursor(0, 1);
  lcd.print("Status: READY");

  pinMode(startButton, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);

  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, LOW);

  Serial.begin(9600);         // For debugging monitor
  slaveSerial.begin(9600);    // For slave communication
}

void loop() {
  if (digitalRead(startButton) == LOW) {
    delay(50);
    if (digitalRead(startButton) == LOW) {
      runMachine();
    }
  }
}

void sendToSlave(String cmd) {
  slaveSerial.println(cmd);     // Send to slave
  Serial.println("[To Slave] " + cmd); // Echo to Serial Monitor
}

void runMachine() {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Washing Machine");
  lcd.setCursor(0, 1);
  lcd.print("Filling Water...");

  sendToSlave("FILL_ON");

  while (true) {
    float level = readWaterLevel();
    lcd.setCursor(0, 1);
    lcd.print("Level: " + String(level, 1) + " cm     ");
    if (level <= 10.0) {
      sendToSlave("FILL_OFF");
      break;
    }
    delay(1000);
  }

  delay(500);
  sendToSlave("MOTOR_ON");

  for (int i = 10; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Washing: " + String(i) + "s     ");
    delay(1000);
  }

  sendToSlave("MOTOR_OFF");
  delay(500);
  sendToSlave("DRAIN_ON");

  for (int i = 5; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Rinsing: " + String(i) + "s     ");
    delay(1000);
  }

  sendToSlave("DRAIN_OFF");
  delay(500);
  sendToSlave("MOTOR_ON");

  for (int i = 10; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Spinning: " + String(i) + "s    ");
    delay(1000);
  }

  sendToSlave("MOTOR_OFF");

  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, HIGH);

  lcd.setCursor(0, 1);
  lcd.print("Status: DONE!   ");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Washing Machine");
  lcd.setCursor(0, 1);
  lcd.print("Status: READY");
}

float readWaterLevel() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  return duration * 0.0343 / 2;
}

code master latest