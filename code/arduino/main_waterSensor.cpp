/*
 * Watersensor and servo
 * Reads the sensor data from a watersensor and prints it out.
 * Also is moving a servo according to the data of the water sensor.
 */

#include "Arduino.h"
#include <Servo.h>

Servo servo;

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  servo.attach(14);

  Serial.begin(115200);
  Serial.println("Starting program");
}

void loop()
{
  int waterSensor = analogRead(A0);
  Serial.println(waterSensor);

  if(waterSensor < 100)
  {
    servo.write(0);
  }
  else if(waterSensor < 200)
  {
    servo.write(90);
  }
  else
  {
    servo.write(180);
  }

  delay(1000); 
 }
