#include <Arduino.h>
#define RESET_PIN 15 //D7
#define POWER_PIN 13 //D8
#define CONFIG_PIN 2  //D4

void pinSetup();
void pinToggle(uint8_t pin, unsigned long ms);

void pinSetup() {
  pinMode(CONFIG_PIN, INPUT);

  pinMode(RESET_PIN,OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  pinMode(POWER_PIN,OUTPUT);
  digitalWrite(POWER_PIN, LOW);
}

void pinToggle(uint8_t pin, unsigned long ms) {
  Serial.printf("*PIN: Closing pin %i for %i ms\n", pin, int(ms));
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
  Serial.println("*PIN: Circuit open.");
}
