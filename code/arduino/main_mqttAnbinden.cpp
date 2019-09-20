/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
#include <Servo.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

WiFiManager wifiManager;
Servo servo;
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  servo.attach(14);

  Serial.begin(115200);
  Serial.println("Starting program");

//Mit dem WLAN verbinden
  WiFi.begin("WILaWiFi063C", "p7-15Dex9.4A");

  client.setServer("10.0.1.53", 1883);
  client.setCallback(callback);
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/nfc/1", "Connected a new water sensor");
      // ... and resubscribe
      client.subscribe("/nfc/callback");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop()
{
  long int waterSensor = analogRead(A0);
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

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  snprintf (msg, 50, "Water #%ld", waterSensor);
  client.publish("/nfc/1", msg);

  delay(5000); 
 }
