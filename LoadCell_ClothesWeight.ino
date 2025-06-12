#include "HX711.h"

#define DOUT 3
#define CLK 2
#define BUZZER_PIN 8

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(DOUT, CLK);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Initializing...");
  delay(1000);

  scale.set_scale();             // You need to calibrate this value
  scale.tare();                  // Reset the scale to 0

  Serial.println("Ready.");
}

void loop() {
  float weight = scale.get_units();  // Get weight in grams (after calibration)
  Serial.print("Weight: ");
  Serial.print(weight, 1);
  Serial.println(" g");

  if (weight > 1000.0) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(500);
}
