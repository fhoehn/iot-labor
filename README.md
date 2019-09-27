



# Inhaltsverzeichnis

- [Inhaltsverzeichnis](#inhaltsverzeichnis)
- [Einführung](#einf%c3%bchrung)
  - [Aufgabenstellung](#aufgabenstellung)
  - [Aufbau der Arbeit](#aufbau-der-arbeit)
  - [Eingesetzte Hardware](#eingesetzte-hardware)
  - [Eingesetzte Software](#eingesetzte-software)
- [Teil 1 - Raspberry, Arduino, NFC und Node-RED](#teil-1---raspberry-arduino-nfc-und-node-red)
  - [Einrichtung des Raspberrys](#einrichtung-des-raspberrys)
  - [Erfassung von NFC-UIDs](#erfassung-von-nfc-uids)
  - [Übertragung der UIDs an den MQTT-Broker](#%c3%9cbertragung-der-uids-an-den-mqtt-broker)
  - [Bereitstellung eines REST Services](#bereitstellung-eines-rest-services)
- [Einbindung eines Arduinos](#einbindung-eines-arduinos)
  - [Aufsetzen einer Entwicklungsumgebung](#aufsetzen-einer-entwicklungsumgebung)
  - [Aufbau einer WLAN-Verbindung](#aufbau-einer-wlan-verbindung)
  - [Anbindung des Wassersensors und Servomotors](#anbindung-des-wassersensors-und-servomotors)
  - [Anbindung von MQTT](#anbindung-von-mqtt)
- [Teil 2 - Integration von OpenHab](#teil-2---integration-von-openhab)
  - [Installation](#installation)
  - [Anbindung einer Hue Bridge und Hue Lampe](#anbindung-einer-hue-bridge-und-hue-lampe)
- [Tag 3 Blockchain Integration](#tag-3-blockchain-integration)
  - [Installation der benötigten Komponente](#installation-der-ben%c3%b6tigten-komponente)
    - [Installation von Truffle](#installation-von-truffle)
    - [Installation von Ganache](#installation-von-ganache)
  - [Einrichtung von Ganache](#einrichtung-von-ganache)
  - [Implementation eines Smart Contracts](#implementation-eines-smart-contracts)
  - [Einbindung der Blockchain ins IOT-Netz](#einbindung-der-blockchain-ins-iot-netz)
    - [Installation Web3](#installation-web3)
    - [Integration der Blockchain](#integration-der-blockchain)
  - [Beschreibung des angestrebten Smart Contract-Szenarios](#beschreibung-des-angestrebten-smart-contract-szenarios)
- [Zusammenfassung und Bewertung der Ergebnisse](#zusammenfassung-und-bewertung-der-ergebnisse)

<div class="page"/>

# Einführung

## Aufgabenstellung

Ziel dieser Arbeit ist es eine IOT-Infrastruktur bereitzustellen, welche auf den drei Schichten (Perception, Network, und Application Layer) einer IOT-Architektur basiert. Dazu sollen verschiedene Sensoren über ein Gateway mit einer Integrationsplattform verbunden werden. Über diese Integrationsplattform soll die Abindung von weiteren Geräten, an das IOT-Netz, erfolgen. Darüber hinaus soll eine Zahlungs- und Abwicklungsmöglichkeit für unterschiedlichste Business Cases eingeführt werden. Um dies vertrauenswürdig und sicher zu gewährleisten, wird eine Blockchain in Kombination mit Smart Contracts eingesetzt.   

## Aufbau der Arbeit
Im ersten Teil des Laborversuchs wird zunächst die Einrichtung des Raspberrys vorgenommen und die für den Aufbau notwendige Infrastruktur aufgesetzt. Anschließend werden mithilfe eines Python-Programms UIDs von NFC-Karten ausgelesen. Diese werden mithilfe des Kommunikationsprotokolls "MQTT" an Node-RED übertragen. Im Node-RED wird daraufhin ein sogenannter Flow implementiert, der den Nachrichtenfluss zwischen zwei Instanzen beschreibt.
Anschließend wird der Laborversuch um weitere Sensoren (Servomotor und Wassersensor) erweitert. Zur Anbindung dieser Sensoren erfolgt der Einsatz eines Microcontrollers (Arduino). Dieser sendet die von den Sensoren übertragenen Nachrichten an die Message Queue, die auf Node-RED ausgewertet werden.
Für die Bereitstellung einer Benutzeroberfläche zur Homeautomatisierung, wird die Integrationsplattform "OpenHab" eingesetzt.
Abschließend soll das Szenario um eine Blockchain erweitert werden, bei der mithilfe von Smart Contracts beispielhafte Geschäftsprozesse im IOT-Umfeld abgewickelt werden können. 
## Eingesetzte Hardware
Für die Durchführung des Laborversuchs wird folgende Hardware benötigt:
-   Raspberry Pi 3
-   Arduino (ESP8266MOD)
-   NFC-Karte
-   Wassersensor
-   Servomotor
-   Philipps Hue Bridge
-   Phillips Hue Lampe
-   Entwicklungsrechner

Zusätzlich werden eine funktionierte WLAN-Verbindung und etwaige Verbindungskabel (Steckbrücken, USB-Kabel usw.) benötigt.

## Eingesetzte Software
Folgende Softwarekomponenten sind für den Laborversuch erforderlich:
-   Raspbian (Betriebssystem)
-   PuTTY
-   Arduino-Treiber für Windows
-   Visual Studio Code inclusive PlattformIO
-   OpenHab
-   Node-RED
-   Truffle
-   Ganache

Neben diesen aufgeführten Softwarekomponenten werden noch weitere Libraries (Web3, WiFi-Manager usw.) benötigt.
<div class="page"/>

# Teil 1 - Raspberry, Arduino, NFC und Node-RED

Zu Beginn des Labors ist zunächst der Raspberry PI für die Entwicklung des Szenarios zu konfigurieren. 
Hierzu muss für den Raspberry eine WLAN-Verbindung aufgebaut und die SSH-Kommunikation aktiviert werden. Auf dieser Weise können zum einen die für die Umsetzung des Szenarios notwendigen Pakete aus dem Internet installiert und zum anderen wird ein Zugriff auf den Raspberry über das Werkzeug "PuTTY" ermöglicht.

## Einrichtung des Raspberrys

Die einzelnen Konfigurationsschritte werden im Folgenden aufgeführt:

1. Aufbau einer WLAN-Verbindung
    1. Da der Raspberry keine Benutzeroberfläche und Eingabemöglichkeit aufweist, wird für die Konfiguration ein Monitor und eine Tastatur an den Raspberry angeschlossen.
    2. Der Raspberry muss via Ethernet an das Netzwerk angeschlossen werden. Hierzu wird die Konfigurationsdatei geöffnet: 
   
            sudo nano /etc/wpa_supplicant/wpa_supplicant.conf 
        In dieser Konfigurationsdatei muss eine Ergänzung der Netzwerkverbindungsdaten erfolgen. Die für diesen Laborversuch notwendigen Netzwerkdaten werden in folgender Abbildung zusammengefasst:
        
            network={
                ssid="WiLaWiFi063C"
                psk="p7-15Dex9.4A"
            }
        
    3. Für die Aktivierung der Änderung muss im nächsten Schritt ein Neustart des Netzwerkmoduls erfolgen. Die für die Ausführung notwendige Kommandozeile wird nachfolgend aufgeführt:
            sudo ifdown wlan0 && duwo ifup wlan0
2. Konfiguration der SSH-Verbindung
    1. Zunächst wird der Raspberry-Konfigurator geöffnet:

        sudo raspi-config
    2. Anschließend muss eine Navigation zum SSH-Eintrag erfolgen und die Eingabe über "Enable" bestätigt werden.
    3. Im nächsten Schritt ist der SSH über das Advanced Packaging Tool (APT) zu installieren:
            
            sudo apt-get install ssh
    4. Der Server kann nun über folgenden Kommandozeilenbefehl erfolgen:
            
            sudo /etc/init.d/ssh start
    5. Zudem ist es hilfreich den automatischen Start der SSH-Funktion zu aktivieren:

            sudo update-rc.d ssh defaults


3. Aufbau einer externen Verbindung über das Werkzeug PuTTY

    Nach dem auf dem Raspberry die Kommunikation über SSH aktiviert wurde, kann über das Werkzeug "PuTTY" eine Verbindung zum Raspberry aufgebaut werden. Für den Aufbau der Verbindung werden die Hostadresse, der Nutzername und das Passwort benötigt. 

4. Installation von Python
    
    Für die weitere Entwicklung sind nun sowohl Python als auch Node-RED zu installieren. Für die Installation dieser Softwarekomponenten wird vom APT Gebrauch gemacht. Wichtig ist, dass die APT zunächst über ein Update auf den aktuellen Stand gebracht wird. Anschließend kann die Installation von Python und Node-RED über folgende Kommandozeile erfolgen: 

        sudo apt-get update && sudo apt-get install python3 python3-pip cmake nodered

5. Installation von NFC-Paketen
   
   Um auf die NFC-Schnittstelle zugreifen zu können, sind spezifische NFC-Pakete zu installieren. Hierzu wird zunächst eine vom Dozenten bereitgestellte ZIP, welches die entsprechenden Pakete enthält, auf das temporäre Verzeichnis des Raspberrys abgelegt. Dies kann über den folgenden Kommandozeilenbefehl erfolgen:

        cd /tmp ; wget https://farberg.de/talks/iot/downloads/nfc_debian_packages.zip
    
   Anschließend wird dieses Paket entpackt und die darin enthaltenen Pakete werden installiert:

        cd /tmp
        unzip nfc_debian_packages.zip
        cd debian_packages 
        sudo dpkg --install *.deb 
        sudo apt-get --fix-broken install

6. Installation eines MQTT-Clients

    Um mit dem Raspberry MQTT-Nachrichten verschicken und empfangen zu können, wird ein MQTT-Client benötigt. Hierzu wird über die folgende Kommandozeile der 'pika-Client' installiert:

        sudo pip3 install nxppy paho-mqtt pika
    
7. Konfiguration eines Serial Peripheral Interface (SPI)

    Der SPI ist ein Interface Bus über den Daten zwischen dem Microcontroller und den Peripheriegeräten, wie beispielsweise Sensoren oder NFC-Karten, ausgetauscht werden können.

## Erfassung von NFC-UIDs

Nach erfolgreiche Konfiguration des Raspberrys, soll im nächsten Schritt ein Programm für das Auslesen von NFC-UIDs geschrieben werden. Die hierfür notwendigen Arbeitsschritte werden nachfolgend aufgeführt.

1. Installation der nxppy-Library
    
    Die nxppy-Library wird zum Einlesen und Schreiben von NFC-Karten benötigt. Die Installation wird über die folgende Kommandozeile umgesetzt: 

        pip install nxppy

2. Ausgabe der NFC-UIDs
   
   Das Auslesen der UIDs von NFC-Karten wird über die Programmiersprache "Python" umgesetzt. Hierzu wurde das folgende Python-Programm 'test1.py', das UIDs von NFC-Karten einliest und auf der Konsole ausgibt, geschrieben:

        test1.py

        import nxppy
        import time

        mifare = nxppy.Mifare()

        # Print card UIDs as they are detected
        while True:
            try:
                uid = mifare.select()
                print(uid)
            except nxppy.SelectError:
                # SelectError is raised if no card is in the field.
                pass

            time.sleep(1)
        
    Nach dem Start des Programms wird jede Millisekunde der aktuelle Wert des NFC-Lesegerätes eingelesen und auf der Konsole ausgegeben. Für die Zustandsänderung kann abwechselnd eine andere NFC-Karte über das NFC-Lesegerät gehalten werden.  


## Übertragung der UIDs an den MQTT-Broker

Basierend auf dem erstellten Python-Programm, das zuständig für das Auslesen der UIDs ist, wird als nächstes die Anbindung eines MQTT-Brokers angestrebt. Ziel ist es die eingelesenen UIDs an den MQTT-Broker zu übertragen. Dazu wird ein [Node-RED](https://nodered.org/) Server verwendet.

1. Installation von Node-RED
   
   Im ersten Schritt ist ein Node-RED MQTT-Broker zu installieren. Hierzu wird der Node Package Manager (NPM) verwendet. Dieser stellt ein Paketmanager für die JavaScript-Laufzeitumgebung 'Node.js' dar. Für die Installation ist folgende Kommandozeile auszuführen:

        npm install node-red-contrib-mqtt-broker

2. Start von Node-RED
   
   Nach der erfolgreichen Installation von Node-RED, wird es im nächsten Schritt gestartet: 

        node-red-start

    Anschließend ist die Node-RED Weboberfläche über die URL `https://<ip>:1880` erreichbar. Da Node-RED auf dem Raspberry installiert ist, ist in diesem Fall die Ip des Raspberrys zu nutzen. 

3. Erstellung eines Flows

    Als nächstes ist ein Flow zu erstellen, welcher Nachrichten vom MQTT-Broker entgegennimmt und im Node-RED als DEBUG-Information ausgibt. Die hierfür notwendigen Schritte sind den nachfolgenden Ausführungen zu entnehmen:
    
    1. Der zu erstellende Flow weist eine MQTT-Eingabe mit einer Verbindung zu einer DEBUG-Ausgabe auf. Diese beiden Bauteile können per Drag and Drop von der Toolbox eingefügt und miteinander verbunden werden. 
    
        ![Screenshot des ersten Flows](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/firstFlow.PNG?raw=true "Erstellter Flow mit Ein- und Ausgabe")

    2. Im nächsten Schritt muss der MQTT-Server eingestellt werden. Dies kann in den Eigenschaften des Inputs erfolgen. 
   
        ![Screenshot Server Einstellen](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/configMqttNode.PNG?raw=true "Einstellen des MQTT-Servers")

    3. Das Topic der Eingabe muss ebenfalls festgelegt werden. Dazu wird in den Eigenschaften, der MQTT-Eingabe, das Topic "/nfc/1" angelegt:
   
        ![Screenshot des Topics](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/configMqttInput.PNG?raw=true "Topic Eingabe")

    4. Anschließend muss der erstellte Flow auf den Raspberry eingespielt werden. Hierbei ist zu beachten, dass jede Änderung ein erneutes Einspielen (engl. Deployen) voraussetzt:

        ![Screenshot des Deploy-Buttons](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/deployFlow.PNG?raw=true "Deploy Flow")

4. Übertragung von Daten an den MQTT Broker

    Nach dem für die Verarbeitung der MQTT-Nachrichten ein Flow erstellt wurde, sollen nun die Daten vom NFC-Lesegerät an die MQTT-Queue, über das Topic "nfc/1/", übertragen werden. Dieser Anwendungsfall wird in einem, weiteren, Python-Programm umgesetzt. Hierzu wird die Datei 'broker.py' erstellt und darin der entsprechende Programmcode implementiert:

        broker.py

        import time
        import nxppy
        import paho.mqtt.client as mqtt

        mifare = nxppy.Mifare()

        mqttsender = mqtt.Client('clientId0123456')
        mqttsender.connect("localhost", 1883)
        mqttsender.subscribe("/nfc/card")

        while True:
            try:
                uid = mifare.select()
                print(mqttsender.publish("/nfc/card", uid))
                print(uid)
            except nxppy.SelectError:
                pass

            time.sleep(0.3)
        
    Dieses Programm stellt eine Erweiterung des zuvor vorgestellten Programms 'test1.py' dar. Im Vergleich zum vorherigen Programm werden die UIDs nicht mehr auf der Konsole ausgegeben, sondern an den MQTT-Broker gesendet. Diese Nachrichten können anschließend von Node-RED abgegriffen werden und der oben vorgestellte Flow wird abgearbeitet. Dieser Vorgang wird für jede, vom Lesegerät empfangene UID, durchgeführt. 
    
5. Einsicht auf DEBUG-Informationen 
   
   Die empfangenen Nachrichten sind auf der Node-RED Oberfläche ersichtlich:

   ![Screenshot der Node-RED DEBUG-Informationen](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/DEBUG_Node-RED.PNG?raw=true "DEBUG-Informationen")

## Bereitstellung eines REST Services

Im Rahmen eines weiteren Anwendungsfalls sollte ein REST Service zur Verfügung gestellt werden. Über diesen REST Service sollen die letzten zehn UIDs abgefragt werden können. Aus zeitlichen Gründen wurde dieser Anwendungsfall nicht durchgeführt. Dies hatte allerdings keinen negativen Einfluss auf den weiteren Verlauf des Laborversuchs. 
<div class="page"/>

# Einbindung eines Arduinos

Der nächste Abschnitt des Laborversuchs befasst sich mit dem Anschluss eines Arduinos (ESP8266MOD) an die bisher erstellte IOT-Infrastruktur. Arduino ist eine aus Software und Hardware bestehende Physical-Computing-Plattform, deren Hardware aus einem Microcontroller und analogen sowie digitalen Ein- und Ausgängen besteht. Ziel des Laborversuchs war es,zunächst, mithilfe des Arduinos eine Lampe an- und ausschalten zu können. Im nächsten Schritt wird der Versuch um den Anschluss eines Wassersensors sowie eines Servomotors ergänzt. Im letzten Schritt soll mithilfe von MQTT der Nachrichtenaustausch vollzogen werden. 

## Aufsetzen einer Entwicklungsumgebung

 Der Beginn der eigentlichen Entwicklung, setzte das Aufsetzen einer Entwicklungsumgebung voraus. Mithilfe dieser Entwicklungsumgebung können C-Code Programme geschrieben und auf den Arduino ausgeführt werden. Die hierfür notwendigen Schritte sind den nächsten Ausführungen zu entnehmen:

1. Installation von Visual Studio Code
    
   Im ersten Schritt wird Visual Studio Code und anschließend im Pluginmanager das Plugin "PlattformIO" installiert.

2. Installation des Treibers

    Das Aufspielen des C-Codes auf den Arduino erfolgt basierend auf einer Verbindung zwischen dem Entwicklungsrechner und dem Arduino. Die Verbindung wird mithilfe eines USB-Kabels umgesetzt. Zusätzlich wird eine manuelle Installation des Arduino-Treibers vorausgesetzt, da der Entwicklungsrechner die Treiber nicht selbstständig beziehen kann. 

3. Erstellung eines Beispielprojekts
   
    Nachdem der richtige Treiber installiert wurde, wird ein Beispielprojekt auf den Arduino aufgespielt. Dieses Beispielprojekt ist dem folgenden Codeauszug zu entnehmen: 

        #include "Arduino.h"

        void setup()
        {

        pinMode(13, OUTPUT);
        }

        void loop()
        {
            digitalWrite(13, HIGH);
            delay(1000);
            digitalWrite(13, LOW);
            delay(1000);
        }

    Die Ausführung des oben stehenden Programmcodes führt dazu, dass die eingebaute LED des Arduinos im sekündlichen Abstand ein- und ausgeschaltet wird.
    
## Aufbau einer WLAN-Verbindung
   
 Weitergehende Szenarien können auch mithilfe von weiteren, am Arduino angeschlossenen, Sensoren umgesetzt werden. Es bietet sich an, die von den Sensoren ermittelten Werte weiterzuverarbeiten. Beispielsweise können gemessene Temperaturunterschiede an den Raspberry gesendet und von da aus Aktionen über Node-RED oder weitere Anwendungen initialisiert werden. Um vom Arduino Nachrichten an den Raspberry schicken zu können, muss allerdings für den Arduino eine Verbindung zu einem WLAN-Netz aufgebaut werden. Hierfür kann beispielsweise ein WiFi-Manager Verwendung finden. Der Wifi-Manager agiert dabei folgendermaßen:
    
- Das Gerät erstellt ein Access Point (neues WLAN-Netz)
- Nach dem Verbinden mit dem WLAN wird man automatisch zu einer Website zum Einloggen weitergeleitet
- Das Gerät listet dort alle WLAN-Netze in der erreichbaren Umgebung auf
- Nun kann eine Verbindung mit dem gewünschten Netzwerk aufgebaut werden
- Handelt es sich beim gewünschten Netzwerk um ein gesichertes Netz, so ist das entsprechende Sicherheitspasswort auf der Weboberfläche zu übergeben
- Das Gerät verbindet sich anschließend mit dem WLAN und speichert die Credentials in einem Secure Storage, sodass das Gerät zukünftig automatisch eine Verbindung zu diesem WLAN-Netz aufbaut
- Ein möglicher Wechsel zu einem anderen Netzwerk setzt das Löschen des internen Speichers voraus

Der WifiManager kann mit folgendem Programmcode auf einfache Art und Weise eingebunden werden. In der autoConnect-Methode des WifiManagers kann der Name und ein Passwort für das Wifi festgelegt werden. Somit wird das neue Wifi vor unberechtigtem Zugriff geschützt. 

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

Die konfigurierte WLAN-Verbindung ermöglicht nun die Sendung der Nachrichten an den MQTT-Broker des Raspberrys. 

## Anbindung des Wassersensors und Servomotors

Nun sollen die Daten des Wassersensors verarbeitet werden. Im ersten Schritt soll ein Servomotor angesteuert werden, der sich um 90 Grad dreht, sobald der Wassersensor sich im Wasser befindet. Ansonsten soll der Motor auf die Ausgangangslage zurück gesteuert werden. Dazu muss zunächst der Aufbau einer Verbindung zwischen den einzelnen Sensoren und dem Arduino erfolgen. Um die Sensoren mit den GPIOs des Arduinos zu verbinden, können Steckbrücken Verwendung finden. Nachdem die Sensoren an den Arduino angeschlossen wurden, kann ein Programm zur weiteren Verarbeitung, wie beispielsweise dem nachfolgend aufgeführten Programmcodeausschnitt, erstellt werden: 

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


## Anbindung von MQTT

 Neben der Schaltung des Servomotors, sollen die Daten des Wassersensors auch über MQTT an den Raspberry übertragen werden. Dazu muss der Arduino die gesammelten Daten verarbeiten und an den Broker senden. Die Umsetzung dieses Anwendungsfalls kann über folgenden Programmcode erfolgen:

    /*
    * MQTT
    * sends and receives data from the mqtt.
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
            digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on 
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

        //Connect with WLAN
        WiFi.begin("<ENTER THE WLAN-ID HERE>", "<ENTER THE PW HERE>");

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

        //Create a message for the sensor data
        snprintf (msg, 50, "Water #%ld", waterSensor);
        //Publish the message to MQTT
        client.publish("/nfc/1", msg);

        //Wait 5 seconds
        delay(5000); 
    }

Die neuen Sensordaten werden alle fünf Sekunden an MQTT geschickt. Ebenso werden alle fünf Sekunden die neuen Nachrichten überprüft. Wenn neue Nachrichten vorhanden sind, werden diese ausgegeben. Wenn das erste Zeichen der empfangenen Nachricht eine "1" ist, wird die eingebaute LED des Arduinos eingeschaltet. Andererseits wird diese ausgeschaltet. Um eine dauerhafte Verbindung mit MQTT sicherzustellen, wird alle fünf Sekunden überprüft, ob eine Verbindung zum MQTT-Server vorhanden ist. Wenn dies nicht der Fall ist, wird versucht eine neue Verbindung aufzubauen. Um das Programm nutzen zu können, müssen die Nutzerdaten des WLANs in der Setup-Methode eingetragen werden. Diese wurden aus Sicherheitsgründen in diesem Beispiel entfernt. 
Nachfolgender Skizzierung ist der aktuelle Aufbau des Laborversuchs zu entnehmen: 

![Arrduino Anbindung an den Raspberry via MQTT](https://github.com/fhoehn/iot-labor/blob/master/images/architecture/arduinoRaspberryConnection.png?raw=true "Arrduino Anbindung an den Raspberry via MQTT")
<div class="page"/>

# Teil 2 - Integration von OpenHab
   
Im nächsten Schritt wird der Laborversuch um die Integrationsplattform „OpenHab“ ergänzt, mit dessen Hilfe die Realisierung einer Homeautomatisierung ermöglicht wird. Dies erfolgt beispielsweise dadurch, dass eine Vielzahl von Bibliotheken mitgeliefert wird. Die Architektur von OpenHab weist einen skalierfähigen Charakter auf und lässt sich mit einem Baukasten vergleichen, da unterschiedliche Technologien und Systeme an die Architektur angebunden werden können. 

Zur Übersicht wird in folgender Abbildung die bisher im Laborversuch aufgebaute IOT-Infrastruktur visualisiert: 

![OpenHAB integration](https://github.com/fhoehn/iot-labor/blob/master/images/architecture/nodeRED_OpenHAB_Integration.png?raw=true "OpenHAB Integration")

Die aufgeführte Abbildung macht deutlich, dass neben dem MQTT-Broker auch Node-RED und OpenHAB auf dem Rapsberry installiert wurden. Allerdings ist für den produktiven Einsatz die Installation aller Komponenten auf einer einzelnen Instanz nicht zu empfehlen. Vielmehr empfiehlt es sich, die einzelnen Softwarekomponenten auf jeweils einem eigenen Gerät zu installieren und die Kommunikation untereinander mittels MQTT zu realisieren. Aufgrund der mangelnden Ressourcen werden die Softwarekomponenten für den Laborversuch jedoch auf einem einzelnen Gerät installiert und nur logisch betrachtet voneinander getrennt. Dieser Umstand stellt für die weitere Versuchsdurchführung jedoch kein Problem dar. 

## Installation

Für die Installation bietet OpenHab eine [Installationsanleitung auf ihrer Website](https://www.openhab.org/download/) an. Im Rahmen der Installation wird zunächst der Repository Key hinzugefügt und auf dem Raspberry der folgende Befehl ausgeführt:

    wget -qO - 'https://bintray.com/user/downloadSubjectPublicKey?username=openhab' | sudo apt-key add

Im nächsten Schritt wird für das APT der HTTPS-Transport aktiviert:

    sudo apt-get install apt-transport-https

Zudem wird auf dem Raspberry das OpenHab-Repository hinzugefügt:

    echo 'deb https://dl.bintray.com/openhab/apt-repo2 stable main' | sudo tee /etc/apt/sources.list.d/openhab2.list

Anschließend erfolgt eine Aktualisierung des APT-Pakets auf dem aktuellsten Stand und das OpenHab Distribution Paket wird installiert:

    sudo apt-get update && sudo apt-get install openhab2

Zusätzlich sind die OpenHab-AddOns zu installieren:

    sudo apt-get install openhab2-addons

Nachdem diese Schritte durchgeführt wurden, hat sich unter Zunahme [einer weiteren Installationsanleitung](https://www.openhab.org/docs/installation/linux.html) ergeben, dass die OpenHab-Installation einen installierten Java JDK 8 voraussetzt. Dementsprechend erfolgte die Installation des Java JDK 8s über folgende Kommandozeile:

    sudo apt-get update && sudo apt-get install open-java8-jdk

Allerdings scheiterte die Installation des OpenHab-Pakets erneut, da das Paket als eine unsichere Paketinstallation angesehen wird. Dementsprechend war der Kommandozeilenbefehl, für das Hinzufügen des Repositorys, mit dem Zusatz [trusted=yes] zu versehen. Auf dieser Weise wird allen Quellen in diesem Repository vertraut. Der mit diesem Zusatz angereicherte Befehl lässt sich wie folgt aufschlüsseln:

    echo 'deb [trusted=yes] https://dl.bintray.com/openhab/apt-repo2 stable main' | sudo tee /etc/apt/sources.list.d/openhab2.list

Anschließend kann die OpenHab-Seite über die Ip und den Port 8080 aufgerufen werden. Da der Port 8080 bereits von einer anderen Anwendung belegt war, musste der Port umgeändert werden. Die Änderung des Ports kann durch eine Anpassung der Konfigurationsdatei, die auf dem Verzeichnis "etc/default" abgelegt ist, erfolgen.


## Anbindung einer Hue Bridge und Hue Lampe

Im nächsten Abschnitt des Laborversuchs wurden den Studenten vom Dozenten mit OpenHab anzubindende Geräte vorgestellt. Für die Durchführung des hier dargestellten Laborversuchs, fiel die Wahl auf die Hue Bridge und die Hue Lampe. Die Hue Bridge verfügt über keine WLAN-Funktion, weshalb ein Ethernet Kabel an das Gerät angeschlossen wird. Anschließend kann die Anbindung der Hue Bridge mit OpenHab und der Steuerung der Hue Lampen erfolgen. Für die Anbindung der Hue Bridge muss im OpenHab die "PaperUI" aufgerufen werden und anschließend über den Reiter Add-Ons das Binding „Hue Binding“ installiert werden. Die nachfolgende Abbildung visualisiert das Hinzufügen des Hue Bindings:
![Screenshot OpenHab Add-Ons Installation](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/AddHueBinding.png?raw=true "Installation eines Hue Bindings")

Anschließend werden unter dem Reiter „Inbox“ alle für die Kopplung verfügbaren Geräte angezeigt:

![Screenshot OpenHab Inbox-Übersicht](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Inbox.png?raw=true "Übersicht der anzukoppelnden Geräte")

Alle verfügbaren [Things](https://www.openhab.org/docs/concepts/things.html) (steuerbare Geräte mit Sensoren, die beispielsweise über "ZigBee" angesprochen werden können) stehen nach Betätigung der Häkchen über den Reiter „Configuration“ zur Konfiguration zur Verfügung:

![Screenshot OpenHab Configuration_Things-Übersicht](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Configuration_Things2.png?raw=true "Übersicht der konfigurierbaren Things")

Für die Anbindung und Steuerung des Things ist ein [Channel](https://www.openhab.org/docs/concepts/things.html) und ein [Item](https://www.openhab.org/docs/configuration/items.html) zu konfigurieren. Während das Thing als eine physische Entität einer Information angesehen werden kann, stellt der Channel eine konkrete Funktion des Things dar. Zudem sind Channels mit Items verlinkt, die wiederum im OpenHab-Umfeld als Ausprägung der Funktion (Channel) zu verstehen sind. So kann beispielsweise das Item "MyNFCThing_" definiert werden, mit dem Item-Typ "String". Dabei gibt der Item-Typ an, dass nur Werte im String-Format entgegengenommen und angezeigt werden können. Der Itemname dient hauptsächlich zur eindeutigen Identifikation des Items (dies ist beispielsweise bei der Implementierung einer Regel von Bedeutung).
Die Channel-konfiguration kann über einen Doppelklick auf dem zu ergänzenden Thing erfolgen. In der aufgepoppten Channel-Konfiguration ist anschließend ein Channelname, Label und der Typ des Channels zu definieren. Zur Verdeutlichung wird im Folgenden eine beispielhafte Channel-Konfiguration aufgeführt:

![Screenshot OpenHab Channel-Konfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Link_Channel.png?raw=true "Beispielhafte Channel-Konfiguration")

Nach Betätigung des im rechten unteren Rand befindlichen „Link“-Buttons, steht das zu steuernde Thing im Reiter "Control" zur Steuerung zur Verfügung:

![Screenshot OpenHab Control-Übersicht](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Overview_Control.png?raw=true "Übersicht der steuerbaren Things")

Als nächstes sollte die Steuerung der Lampe über das Kommunikationsprotokoll „MQTT“ umgesetzt werden. Ziel war es,zunächst, die Hue-Lampe, abhängig vom gemessenen Feuchtigkeitswert (Wassersensor), ein- oder auszuschalten.
Für die Kommunikation über MQTT kann das auf OpenHab verfügbare MQTT-Binding genutzt werden, dessen Installation erneut über den Reiter „Add-Ons“ umgesetzt wird. Als nächstes ist unter dem Reiter „Inbox“ ein neuer Eintrag zu erstellen, der zu folgenden Auswahlmöglichkeiten führt:

![Screenshot OpenHab MQTT Thing Binding-Konfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Options_MQTT_ThingBinding.png?raw=true "Konfigurationsmöglichkeiten vom MQTT-Binding")

Für den oben genannten Anwendungsfall werden lediglich ein MQTT Broker und ein Generic MQTT Thing benötigt. Dementsprechend wird als erstes ein MQTT Broker erstellt. Dem Broker sind anschließend ein Name und die IP-Adresse des Raspberrys zu übergeben. Als Port wird die 1883 gewählt. Die hier beschriebenen Brokereinstellungen sind der nächsten Abbildung zu entnehmen:

![Screenshot OpenHab MQTT Broker-Konfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Configuration_MQTT_Broker.png?raw=true "Konfiguration eines MQTT-Brokers")

Nachdem der MQTT Broker konfiguriert ist, ist im Anschluss ein Generic MQTT Thing zu erstellen. Diesem Thing ist ein Name zu übergeben und einem Broker zuzuweisen. Nach diesen beiden Schritten, können dem neu angelegten MQTT Thing mehrere Channels zugewiesen werden. Für diesen Channel werden folgende Angaben vergeben:

    •	Channel-Typ: Erwartetes Rückgabeformat (beispielsweise text value)
    •	Channel-ID: Eindeutiger Name oder ID (zur eindeutigen Identifierung des Channels) 
    •	MQTT Topic: Als Topic wird das beim Auslesen des Wassersensors verwendete Topic „/water/out“ gewählt

Um nun die Lampenbeleuchtung über MQTT zu steuern, muss eine Regel unter etc/openhab2/rules/default.rules erstellt und beispielsweise mit dem folgenden Code versehen werden:

    rule "Water_Lamp_on"
    when
            Item GernericMQTTThing_ changed
    then
            if(GenericMQTTThing_.state > 200){
                    Lamp1.sendCommand(ON)
            }
            if(GenericMQTTThing_.state <= 199){
                    Lamp1.sendCommand(OFF)
            }
    end

Diese Regel definiert, dass sobald das MQTT THing einen Wassersensor-Wert von über 200 empfängt, eine Anweisung zur Einschaltung der Lampe erfolgt. Dagegen wird die Ausschaltung der Lampe bei einem darunter liegendem Wert ausgelöst. Der Name des zu steuernden Things entspricht dem vergebenen Itemnamen. Dieser Name kann beispielsweise auch über den Aufruf der Openhab-log ermittelt werden:

    Openhab-cli showlogs

Im nächsten Schritt wurde der Aufbau des Anwendungsfalls angepasst. So wurde nun abhängig von dem ausgegebenen Wassersensor-Wert die Lampenbeleuchtungsstärke um einen entsprechenden prozentualen Wert verringert oder erhöht. Zudem wird die simulierte IOT-Architektur um eine Sicherungsschicht ergänzt, indem die Schaltung nur von einer autorisierten Person, identifizierbar über die NFC-UID (im vorliegenden Beispiel "043E436A643481"), ausgeführt werden kann:

    rule "Water_Lamp_on"
    when
            Item GernericMQTTThing_ changed
    then
            if(GenericMQTTThing_.state > 200){
                    Lamp1.sendCommand("100%")
            }
            if(GenericMQTTThing_.state <= 199){
                    Lamp1.sendCommand(50%)
            }
    end

    rule "Water_Lamp_on_2"
    when
            Item MyNFCThing_ changed
    then
            if(MyNFCThing_.state == '043E436A643481'){
                    Lamp2.sendCommand("ON")
            }
            else{
                    Lamp2.sendCommand(OFF)
            }
    end

Die folgende Abbildung gibt visualisiert die Zustandsänderungen Lampenbeleuchtung an. Für diesen Versuch wurde der Wassersensor im Wechsel befeuchtet und getrocknet. Zu beachten ist, dass die Beleuchtungsänderung nur über die autorisierte NFC-Karte erfolgen kann:
![Screenshot OpenHab Zustandänderung-Lampe](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/LampsLighting_Change.png?raw=true "Zustandsänderung der Lampenbeleuchtung")
<div class="page"/>

# Tag 3 Blockchain Integration

Nachdem in den vorherigen Teilen Zustandsänderungen von Sensoren mittels MQTT verarbeitet werden konnten, soll nun eine Blockchain Integration durchgeführt werden. Ziel dieser Integration ist die erfolgreiche Etablierung eines Smart Contracts. Im vorliegenden Beispiel die Entwicklung eines Smart-Contracts angestrebt, der dem Nutzer, für jedes Schalten der Lampe, einen gewissen Geldbetrag in Rechnung stellt. Die Geldbetragsmenge ergibt sich aus dem gemessenem Wassersensor-Wert. 
Um diese Szenario umsetzen zu können, sollen die Smart-Contracts von Ethereum eingesetzt werden. Dabei soll bei Übertragung der NFC-UID zunächst geprüft werden, ob dem entsprechenden Nutzer ausreichend Guthaben zur Verfügung steht. Im positiven Fall wird das für die Schaltung der Lampe notwendige Guthaben, vom Nutzerkonto auf das Betreiberkonto übertragen. Nach der erfolgreichen Übertragung des Guthabens, erfolgt anschließend die Schaltung der Lampe. 

## Installation der benötigten Komponente

Für die Abbildung des oben geschilderten Szenarios wird Truffle zur Entwicklung und zum Test von Smart Contracts und Ganache zur Simulation einer Blockchain verwendet. Diese beiden Komponenten werden auf einen separaten, und somit außerhalb des Raspberrys, installiert. Im Folgenden werden zunächst die für die Installationen notwendigen Schritte geschildert. Anschließend wird auf die Einrichtung von Ganache und Truffle eingegangen und eine Beispielimplementierung für ein Smart Contract aufgeführt.

### Installation von Truffle 

Bevor mit der Installation von Truffle begonnen werden kann, muss zunächst Node.js installiert werden. Dazu wird die neuste Version von der [Node.js-Homepage]((https://nodejs.org/download)) heruntergeladen. 
Als nächstes muss [Truffle](https://www.trufflesuite.com/) installiert werden. Dazu führen wird folgender Befehl über NPM ausgeführt:
    npm install truffle -g

### Installation von Ganache

Die Installation von [Ganache](https://www.trufflesuite.com/ganache) erfolgt ebenfalls über die Homepage.

## Einrichtung von Ganache

Als nächstes muss die Software eingerichtet werden. Hierzu wird die installierte Applikation "Ganache" geöffnet, ein neuer Workspace angelegt und eine Verbindung zu dem Server wird hergestellt (im vorliegenden Fall wird eine Verbindung über den "Localhost" aufgebaut, die Blockchain kann aber auch auf einem separaten Server installiert werden). 

![Screenshot Ganache Serverkonfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/ganache/ganacheServerConfiguration.PNG?raw=true "Serverkonfiguration")

Nach dem Speichern des Workspaces öffnet sich eine Liste der aktuell verfügbaren Accounts, die standardmäßig auf der simulierten Blockchain hinterlegt sind: 

![Screenshot Accounts in Ganache](https://github.com/fhoehn/iot-labor/blob/master/images/ganache/ganacheListWithAccounts.PNG?raw=true "Liste der Accounts mit ihrem aktuellen Guthaben")


## Implementation eines Smart Contracts

Für die Implementierung des Smart Contracts musste zunächst ein Projekt erstellt werden. Hierzu muss ein Verzeichnis erstellt und folgender Befehl innerhalb des Verzeichnisses ausgeführt werden:

    truffle init

Mit diesem Befehl wird anschließend die komplette Verzeichnisstruktur für die Implementierung des Smart Contracts initialisiert. Im nächsten Schritt soll ein neuer Smart Contract hinzugefügt werden. Dafür wird im Contracts-Verzeichnis eine neue Datei mit der Endung ".sol" angelegt. Im vorliegenden Fall wird der Smart Contract mit dem Namen "Light_New" erstellt. Der Smart Contract wird basierend auf der Programmiersprache ["Solidity"](https://www.trufflesuite.com/docs/truffle/getting-started/compiling-contracts) implementiert. 
Zu Beginn wurde testweise ein simpler Smart Contract definiert, der die Nachricht "Hello World" zurückgibt. Ein hierfür beispielhafter Programmcode lässt sich folgendermaßen zusammenfassen:

    pragma solidity ^0.5.0;

    contract Light_New {

        constructor() public {
            owner = msg.sender;

        }

        string private message = "Hello World";

        function getMessage() public view returns(string memory) {
            return message;
        }

        function setMessage(string memory newMessage) public {
            message = newMessage;
        }
    }

Um diesen Smart Contract auf die BlockChain zu übertragen, muss der Smart Contract kompiliert und migriert werden. Hierfür werden folgende Truffle-Kommandobefehle verwendet:
    truffle compile
    truffle migrate

Nach der Ausführung dieser Kommandobefehle, steht der Smart Contract auf der BlockChain zur Verfügung. 


## Einbindung der Blockchain ins IOT-Netz

Für die Anbindung des IOT-Netzes mit dem simulierten Blockchain-Netzwerk, wird die [Web3.js-Library](https://web3js.readthedocs.io/en/v1.2.1/) verwendet. 

### Installation Web3

Für deren Installation kann erneut auf das NPM zurückgegriffen werden. Über den folgenden Befehl wird die Nutzung von Web3.js bereitgestellt:
    npm install web3
Zu berücksichtigen ist, dass hierfür Python mit der Version 2.7 benötigt wird. Die Installation mit einer neueren Python-Version hat im hier beschriebenen Laborversuch zu Problemen geführt. Als spezifischen Lösungsansatz wurde hierzu auf [Anaconda](https://www.anaconda.com/) zurückgegriffen, das die Nutzung der benötigten Version ermöglicht. Für die Installation von Anaconda wurden zusätzlich aktuelle visual studio build tools benötigt, die über folgende Kommandozeile installiert wurden:
    npm install –g windows-build-tools

### Integration der Blockchain

Nach der Installation der benötigten Bibliotheken, kann mit der Integration der Blockchain in das IOT-Scenario begonnen werden. Dazu wird auf dem zu entwickelten Rechner eine neue Javascript-Datei erstellt. In dieser Datei wird ein Programm implementiert, das die Integration zwischen der simulierten Blockchain und der bestehenden IOT-Infrastruktur herstellen soll. Hierzu wird zunächst eine Verbindung zur Blockchain und dem MQTT-Broker hergestellt. Zudem soll bei eingehenden Nachrichten, die im vorliegenden Fall über das abonnierte Topic "water/out" eintreffen, der Smart-Contract aus der Blockchain ausgeführt werden. Der nachfolgende Programmcodeausschnitt visualisiert die technische Umsetzung des soeben beschriebenen Szenarios:

    var mqtt    = require('mqtt');
    const Web3 = require('web3');

    const web3 = new Web3(new Web3.providers.HttpProvider("http://127.0.0.1:7545"));

    var client  = mqtt.connect("mqtt://10.0.1.53",{clientId:"mqttjs01"});
    console.log("connected flag  " + client.connected);

    //handle incoming messages
    client.on('message',function(topic, message, packet){
        console.log("message is "+ message);
        console.log("topic is "+ topic);
        executedSmartContractAsync();
    });

    //handle connected
    client.on("connect",function(){	
        console.log("connected  "+ client.connected);
    });
    //handle errors
    client.on("error",function(error){
        console.log("Can't connect" + error);
        process.exit(1);
    });


    var options={
        retain:true,
        qos:1
    };

    var topic="/water/out";
    console.log("subscribing to topics");
    client.subscribe(topic,{qos:1});


    var fs = require('fs');
    var json = JSON.parse(fs.readFileSync('<PATH_TO_PROJECT>/ethereum/build/contracts/Light_New.json').toString());
    var abi = json['abi'];

    async function executedSmartContractAsync()
    {
        contract = new web3.eth.Contract(abi, "0x1B214466430f5Bf1D92fD60c506c15C5879d66f9");
        console.log(contract);
        let returnedMessage = await contract.methods.getMessage().call();
        
        console.log("Contract answered: ",returnedMessage);
    }
    executedSmartContractAsync();

    console.log("end of script");

Im vorliegenden Programmcode werden zunächst die Module "MQTT" und "Web3" geladen. Anschließend erfolgt mithilfe von Web3 ein Verbindungsaufbau mit der lokalen Blockchain. Zudem wird über MQTT eine Verbindung mit dem Raspberry aufgebaut. 
Bei eingehenden Nachrichten wird eine Ausgabe auf der Konsole erzeugt und die executedSmartContractAsync()-Methode ausgeführt. Um auf eingehende Sensordaten reagieren zu können, Subscribed sich das Programm auf das Topic "/water/out". Bevor der Smart Contract ausgehführt werden kann, muss die ABI (Application binary interface) des Smart Contracts eingelesen werden. Diese wird benötigt, um auf die Schnittstelle zugreifen zu können. Die ABI des Smart Contracts wird beim Kompilieren erzeugt und im Build-Verzeichnis abgelegt. Dementsprechend wird für die Ermittlung der ABI ein Pfad zu dem Build-Verzeichnis hinterlegt und die ABI ausgelesen.
Mithilfe der ABI kann nun auf die getMessage()-Methode des Smart-Contracts zugegriffen werden. 
Für die Ausführung des Smart Contracts muss das Programm zunächst gestartet werden. Sobald Sensordaten über MQTT eintreffen wird auf der Konsole folgendes ausgegeben:

    Contract answered: Hello World


## Beschreibung des angestrebten Smart Contract-Szenarios

Aus zeitlichen Gründen konnte das ursprünglich angedachte Smart Contract-Szenario nicht fertiggestellt werden. Der angestrebte Smart Contract sollte das im Kapitel "OpenHab" beschriebene Szenario dahingehend erweitern, dass relativ zur prozentualen Beleuchtungserhöhung der entsprechende Betrag auf der Blockchain abgezogen wird. So wird beispielsweise bei einem Beleuchtungserhöhung von 100%, 1 Ether abgerechnet, während eine Beleuchtungserhöhung von 10% Kosten in Höhe von 0,1 Ether zur Folge haben. Die Zuordnung für das abzurechnende Konto, erfolgt über die NFC-UID und die Beleuchtungsänderung über den aktuell gemessenen Wassersensor-Wert.
Das angestrebte Szenario lässt sich mit folgender Abbildung und schrittweiser Beschreibung verdeutlichen:

![Blockchain Integration](https://github.com/fhoehn/iot-labor/blob/master/images/architecture/blockchain_Integration.png?raw=true "Blockchain Integration")

1. Der Arduino frägt alle fünf Sekunden den aktuellen Wasserstand des Wassersensors ab
2. Der Arduino sendet über MQTT den aktuellen Wasserstand an den Raspberry
3. Der Raspberry übergibt den aktuellen Wert an den Smart Contract in der Blockchain. Dadurch wird der aktuelle Preis im Smart Contract definiert. 
4. Wenn nun ein Nutzer seine NFC-Karte auf das Lesegerät legt, wird die UID der Karte ausgelesen und der Prozess initialisiert.
5. Der Raspberry versucht den Smart Contract auszuführen. Dazu wird die UID der Karte genutzt, um nachzuschauen, ob der Nutzer über genügend Guthaben verfügt. Im positiven Fall wird das für die Schaltung der Lampe notwendige Guthaben vom Nutzerkonto abgezogen und der Prozess zum Einschalten der Lampe auf dem Raspberry initialisiert.
6. Der Raspberry sendet über MQTT eine Nachricht, bezüglich der der Schaltung der Lampe, an den Arduino.
7. Der Arduino schaltet die Lampe auf den aktuellen Wert.
<div class="page"/>

# Zusammenfassung und Bewertung der Ergebnisse
Zu Beginn des Laborberichts wurde die Konfiguration des Raspberrys vorgenommen. Hierbei konnten nach anfänglichen Schwierigkeiten alle notwendigen Konfigurationsschritte vollzogen werden. Als weniger problematisch stellte sich das anschließende Schreiben eines Python-Programms für das Auslesen von NFC-UIDs dar. Basierend auf diesem Programm sollte im nächsten Schritt eine Übertragung der UIDs an einen MQTT-Broker erfolgen. Hierzu wurde zunächst Node-RED installiert. Auf der Weboberfläche von Node-RED erfolgte daraufhin die Erstellung eines Flows, welches Nachrichten vom MQTT-Broker durch ein Abonnement auf eine Message-Queue entgegennimmt. Mithilfe eines weiteren Python-Programms konnte anschließend realisiert werden, dass die UIDs an den MQTT Broker übergeben und auf der Node-RED Oberfläche als DEBUG-Information angezeigt werden. Die zusätzliche Bereitstellung eines REST-Services, das die letzten zehn übertragenen UIDs bereitstellt, konnte aus zeitlichen Gründen nicht umgesetzt werden.
Im nächsten Schritt wurde der Laborversuch um die Anbindung eines Microcontrollers (Arduino) ergänzt. Dies setzt den Aufbau einer Entwicklungsumgebung (Visual Studio Code) und für den Nachrichtenaustausch mit einem definierten Netzwerk, eine WLAN-Verbindung voraus. Um den Nachrichtenaustausch zwischen dem Arduino und den beiden Sensoren herzustellen, wurde eine Verbindung mithilfe von Steckbrücken hergestellt. Mithilfe eines C-Programmes wurde der Servomotor so programmiert, dass er abhängig vom empfangenen Wassersensor-Wert gedreht wird. Darüber hinaus erfolgte eine erfolgreiche Anbindung des Arduinos mit dem MQTT-Broker.

Im nächsten Abschnitt des Laborversuchs wurde die Integrationsplattform "OpenHab" für die Homeautomatisierung eingeführt. In diesem Kontext wurden eine Hue Bridge und eine Hue Lampe an die simulierte IOT-Infrastruktur angebunden. Durch die von OpenHab mitgelieferten Add-Ons gestaltete sich eine Steuerung dieser Geräte als simpel. So wurde beispielsweise mithilfe einer Regel umgesetzt, dass abhängig vom empfangenen Wassersensor-Wert die Beleuchtungsstärke der Hue Lampe verringert oder erhöht wird. Erweitert wurde dieser Anwendungsfall zusätzlich dahingehend, dass die Steuerung lediglich von einer autorisierten Person (NFC-UID) ausgeführt werden kann.

Im letzten Abschnitt des Laborversuchs sollte die simulierte IOT-Infrastruktur um eine Blockchain-Anbindung angereichert werden. Hierfür erfolgte zunächst die Installation von Truffle, als eine Entwicklungsumgebung für Smart Contracts, und Ganache, das zur Simulation einer Blockchain eingesetzt werden kann. Im Anschluss sollte ein Smart Contract implementiert werden, der bei Schaltung der Lampe zunächst prüft ob der schaltende Nutzer über genügend Guthaben für diese Aktion verfügt und im positiven Fall den entsprechenden Betrag vom Nutzerguthaben abzieht. Aufgrund dessen, dass während der Installationen unterschiedlichste Probleme auftraten (beispielsweise inkompatible Python-Version), konnte das angestrebte Smart Contract-Szenario nicht umgesetzt werden. Dementsprechend wurde im letzten Schritt eine Abbildung vorgestellt, das die geplante technische Umsetzung des Smart Contracts visualisiert. Darüber hinaus erfolgte die Vorstellung des bis zum Ende des Laborversuchs erstellten Smart Contracts.

Unabhängig von der gescheiterten Implementierung des Smart Contracts kann insgesamt von einem erfolgreichen und lehrreichen Laborversuch gesprochen werden. Der Laborversuch macht deutlich, dass mithilfe von geeigneter Hardware und Software sowie durch intensiver Auseinandersetzung mit der Thematik, bereits in kurzer Zeit eine IOT-Infrastruktur aufgesetzt werden kann. Der Laborversuch zeigt auch, dass die verschiedenen Softwarekomponente den größten Entwicklungsaufwand übernehmen, dass deren Installation aber unter Umständen mühselig und zeitraubend seien kann (beispielsweise bei inkompatibler Python-Version). Zudem ist zu berücksichtigen, dass zumindest Grundkenntnisse in/ allen relevanten Programmiersprachen erforderlich sind. Andernfalls sind bei einem eingeschränkten Zeitrahmen, zeitliche Engpässe zu erwarten, wie es beispielweise bei der Programmierung des Smart Contracts der Fall war.
