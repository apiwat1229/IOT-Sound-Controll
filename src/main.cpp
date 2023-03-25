#include <Arduino.h>
#define led_Red 25
#define led_Green 26
#define led_Blue 4

void setup()
{
  Serial.begin(115200);

  pinMode(led_Red, OUTPUT);
  pinMode(led_Green, OUTPUT);
  pinMode(led_Blue, OUTPUT);

  digitalWrite(led_Red, HIGH);
  digitalWrite(led_Green, HIGH);
  digitalWrite(led_Blue, HIGH);
}

void loop()
{
  digitalWrite(led_Green, LOW);
  delay(500);
  digitalWrite(led_Green,HIGH);
  delay(500);
}