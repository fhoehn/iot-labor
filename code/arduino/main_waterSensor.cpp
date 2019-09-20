/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
#include <Servo.h>
//#include <WiFiManager.h>

//WiFiManager wifiManager;
Servo servo;

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  servo.attach(14);

  Serial.begin(115200);
  Serial.println("Starting program");

  //WiFi.begin("WILaWiFi063C", "p7-15Dex9.4A");
  //wifiManager.autoConnect("WILaWiFi063C", "p7-15Dex9.4A");  
  //wifiManager.autoConnect("TEST1", "test");  
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
