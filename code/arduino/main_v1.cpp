/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
#include <Servo.h>

Servo servo;

void setup()
{

  servo.attach(14);

  Serial.begin(115200);
  Serial.println();

}

void loop()
{
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);



 
  int waterSensor = analogRead(D4);
  Serial.print(waterSensor);

  delay(1000);
}
