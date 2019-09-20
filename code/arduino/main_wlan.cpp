/*
 * WiFiManager
 * Opens a new Wifi which can be used for entering the connection details of the WIFI which should be used.
 */

#include "Arduino.h"
#include <WiFiManager.h>

WiFiManager wifiManager;

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Starting program");

  //Because we want to have a secured device, we are using a default password
  wifiManager.autoConnect("TEST1", "test123");  
}

void loop()
{
	//Nothing to do here
	delay(1000); 
}
