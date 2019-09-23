# Laborbericht 
Laborbericht für das Mastermodul 'Internet of Things'

# Kurzzusammenfassung

# Einführung

## Aufgabenstellung

Beschreibe die Aufgabe. Untersuche das Problem, das du lösen oder testen willst. Dies ist das „Ziel“ des Experiments. Warum machst du dieses Experiment? Was kann man von diesem Experiment lernen?[3] Wenn du das Ziel des Experiments erklärst, beschreibe, worum es sich in diesem Experiment handelt und was du untersuchen willst.
Dieser Schritt sollte das Experiment einleiten. Beschreibe Hintergrundinformationen, die für das Experiment relevant sind, wichtige Definitionen, theoretischen und historischen Hintergrund und allgemeine Methoden, die angewendet werden.[4]
Das Ziel sollte in einem Satz dargelegt werden. Dies kann auch als eine Frage formuliert werden. Manchmal wird der Lehrer dir das Ziel des Experiments vorgeben.[5]
Ein Beispiel einer Zielerklärung wäre: Das Ziel des Experiments ist es, anhand drei verschiedener Proben den Siedepunkt von drei verschiedenen Substanzen zu ermitteln.
Ein Beispiel einer Zielfrage wäre: Wird rote und blaue Farbe vermischt zu grüner Farbe?
Tag 1 wollen wir die ID auslesen

....

..

..

## Vorgehensweise
Aufbau der Kapitel

## Eingesetzte Hardware
Beschreibender Text
Alle Hardware Wassersensor, Phillips Hue, Hue Bridge

## Eingesetzte Software
Beschreibender Text
OpenHab, Nodered, Ganache, Truffle, Visual Studio Code
# Teil 1 - NFC und MQTT

Zu Beginn des Labors ist zunächst der Raspberry PI für die Entwicklung des Szenarios zu konfigurieren. 
Hierzu muss für den Raspberry Pi eine WLAN-Verbindung aufgebaut und die SSH-Kommunikation aktiviert werden. Auf dieser Weise können zum Einen die für die Umsetzung des Szenarios notwendigen Pakete aus dem Internet installiert und zum Anderen wird eine Zugriff auf den Raspberry Pi über das Werkzeug PuTTy ermöglicht.

## Raspberry einrichten

Die einzelnen Konfigurationsschritte werden im Folgenden aufgeführt:

1. Aufbau einer WLAN-Verbindung
    1. Da der Raspberry Pi keine Benutzeroberfläche und Eingabemöglichkeit aufweist, wird für die Konfiguration ein Monitor und eine Tastatur an den Raspberry angeschlossen.
    2. Der Raspberry muss via Ethernet an das Netzwerk angeschlossen werden. Dazu muss die Konfigurationsdatei geöffnet werden. 
   
            sudo nano /etc/wpa_supplicant/wpa_supplicant.conf 
        Diese Konfigurationsdatei muss um die Netzwerkverbindungsdaten ergänzt werden. Die für diesen Laborversuch notwendigen Netwerkdaten werden in folgender Abbildung zusammengefasst:
        
            network={
                ssid="WiLaWiFi063C"
                psk="p7-15Dex9.4A"
            }
        
    3. Für die Aktivierung der Änderung muss im nächsten Schritt ein Neustart des Netwerkmoduls erfolgen. die entsprechende Kommandozeile wird in folgender Abbildung aufgeführt:
            
            sudo ifdown wlan0 && duwo ifup wlan0
2. Konfiguration der SSH-Verbindung
    1. TODO SSH konfigurieren

3. Aufbau einer externen Verbindung über das Werkzeug PuTTY

    Nach dem auf dem Raspberry die Kommunikation über SSH aktiviert wurde, kann über das Werkzeug PuTTY eine Verbindung zum Raspberry aufgebaut werden. Für den Aufbau der verbindung wird die Hostadresse, der Nutzername und das Passwort benötigt. 

4. Installation von Python
    
    Für die weitere Entwicklung werden nun sowohl Python als auch nodered installiert. Für die Installation dieser Softwareanwendungen wird vom Advanced Packaging Tool (APT) Gebrauch gemacht. Wichtig ist, dass die APT zunächst über ein update auf den aktuellen Stand gebracht wird. Anschließend kann die Installation von python und nodered über folgende Kommandozeile erfolgen: 

        sudo apt-get update && sudo apt-get install python3 python3-pip cmake nodered

5. Installation von NFC-Paketen
   
   Um auf die NFC-Schnittstelle zugreifen zu können, sind spezifische NFC-Pakete zu installieren. Hierzu wird zunächst eine vom Dozenten bereitgestellte ZIP, welches die entsprechenden Pakete enthält, auf das temporäre Verzeichnis des raspberry abgelegt. Dies kann über folgenden Kommandozeilenbefehl umgesetzt werden:

        cd /tmp ; wget https://farberg.de/talks/iot/downloads/nfc_debian_packages.zip
    
   Anschließend wir dideses Paket entpackt und die darin enthaltenen Pakete installiert:

        cd /tmp
        unzip nfc_debian_packages.zip
        cd debian_packages 
        sudo dpkg --install *.deb 
        sudo apt-get --fix-broken install

6. MQTT Client installieren

    Um später mit dem Raspberry MQTT-Nachrichten verschicken und empfangen zu können, wird ein MQTT-Client benötigt. Hierzu wird über die folgende Kommandozeile der 'pika-Client' installiert:

        sudo pip3 install nxppy paho-mqtt pika
    
7. Konfiguration eines Serial Peripheral Interface (SPI)

    Der SPI ist ein interface bus über den Daten zwischen den Microcontroller und Peripheriegeräten, wie beispielsweise Sensoren oder NFC-Karten, ausgetauscht werden können.

## NFC einlesen

Nach erfolgreiche Konfiguration des Raspberrys, soll im nächsten Schritt ein Programm für das Auslesen von NFC-IDs geschrieben werden. Die hierfür notwendigen Arbeitsschritte werden im Folgenden chronologisch aufgeführt.

1. Installation der nxppy-Library
    
    Die nxppy-Library wird zum einfachen Einlesen und Schreiben von NFC-Chips benötigt. Die installation wird über die folgende Kommandozeile umgesetzt: 

        pip install nxppy

2. Ausgabe der UID von NFC-Chips
   
   Das Auslesen der UID eines NFC-Chips wird über die Programmiersprache Python umgesetzt. Hierzu wurde das folgende Python-Programm 'test1.py', das UID eines NFC-Chips einliest und auf der Konsole ausgibt:

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
        
    Nach dem Start des Programms wird jede Millisekunde der aktuellen Wert des NFC-Lesegerätes eingelesen und auf der Konsole ausgegeben. Für die Zustandsänderung kann abwechselnd eine anderer NFC-Chip über das NFC-Lesegerät gehalten werden.  


## Übertragung der UIDs an den MQTT-Broker

Basierend auf dem erstellten Python-Programm für das Auslesen der UIDs, wird als nächstes die Anbindung eines MQTT-Brokers angestrebt. Ziel ist es, dass die eingelesenen UIDs an den MQTT-Broker übertragen werden. Dazu wird ein Node-RED Server verwendet. [Mehr Details über Node-RED.](https://nodered.org/)

1. Installation von NODE-RED
   
   Im ersten Schritt ist ein Node-RED MQTT-Broker zu installieren. Hierzu wird der Node Package Manager(npm) verwendet. Dieser stellt ein Paketmanager für die JavaScript-Laufzeitumgebung 'Node.js' dar. Für die Installation ist folgende Kommandozeile auszuführen:

        npm install node-red-contrib-mqtt-broker

2. Start von NODE-RED
   
   Nach der erfolgreichen Installation von Node-RED, wird es im nächen Schritt gestartet: 

        node-red-start

    Anschließend ist die Node-RED Weboberfläche über die URL `https://<ip>:1880` erreichbar. Da NODE-RED auf dem Raspberry installiert ist, ist in diesem Fall die Ip des Raspberrys zu nutzen. 

3. Flow-Erstellung

    Als nächstes ist ein Flow zu erstellen, welcher Nachrichten vom MQTT-Broker entgegennimmt und im Node-RED als DEBUG-Information ausgibt. Die hierfür notwendigen Schritt sind den nachfolgenden Ausführeungen zu entnehmen.
    
    1. Der zu erstellende Flow weist eine MQTT-Eingabe mit einer Verbindungen zu einer Debug-Ausgabe auf. Diese beiden Bauteile können per Drag and Drop von der Toolbox eingefügt und miteinander vernunden werden. 
    
        ![Screenshot des ersten Flows](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/firstFlow.PNG?raw=true "Erstellter Flow mit Ein- und Ausgabe")

    2. Im nächsten Schritt muss der MQTT-Server eingestellt werden. Dies kann in den Eigenschaften des Inputs erfolgen. 
   
        ![Screenshot Server Einstellen](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/configMqttNode.PNG?raw=true "Einstellen des MQTT-Servers")

    3. Das Topic der Eingabe muss ebenfalls festgelegt werden. Dazu wird in den Eigenschaften der der MQTT-Eingabe das Topic "/nfc/1" angelegt.
   
        ![Screenshot des Topics](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/configMqttInput.PNG?raw=true "Topic Eingabe")

    4. Anschließend muss der erstellte Flow auf den Raspberry eingespielt werden. Hierbei ist zu beachten, dass jede Änderung ein erneutes deployen voraussetzt.

        ![Screenshot des Deploy-Buttons](https://github.com/fhoehn/iot-labor/blob/master/images/nodeRED/deployFlow.PNG?raw=true "Deploy Flow")

4. Übertragung von Daten an den MQTT Broker

    Nach dem erfolgreichen Erstellen des Flows, für die Verarbeitung der MQTT-Nachrichten, sollen nun die Daten vom NFC-Lesegerät an die Queue übertragen werden. Dieser Anwendungsfall wird in einem, weiteren, Python-Programm umgesetzt. Hierzu wird die Datei 'broker.py' erstellt und darin der entsprechende Programmcode implementiert:

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
        
    Diese Programm stellt eine Erweiterung des zuvor vorgestellten Programms 'test1.py' dar. Im Vergleich zum vorherigen Programm werden die UIDs nicht mehr auf der Konsole ausgegeben, sondern an den MQTT-Broker gesendet. Diese Nachrichten können anschließend von Node-RED abgegriffen werden und der oben vorgestellte Flow wird abgearbeitet. Dieser Vorgang wird für jede, vom Lesegerät empfangene UID, durchgeführt. 

5. Einsicht auf DEBUG-Informationen
   
   Zu erwähnen ist, dass die emfangenen Nachrichten auf der NODE-RED Oberfläche angezeigt werden.

   #TODO[Add Image here]

## Bereitstellung eines REST Services

Im Rahmen eines weiteren Anwendungsfalls sollte ein REST Service zur Verfügung gestellt werden. Über diesen REST Service sollen die letzten 10 UIDs abgefragt werden können. Aus zeitlichen Gründen wurde dieser Anwendungsfall nicht durchgeführt. Dies hatte allerdings keinen negativen Einfluss auf den weiteren Verlauf des Laborversuchs. 

# Teil 2 - Arduino

Der nächste Abschnitt des Laborversuchs befasst sich mit dem Anschluss eines Arduinos (ESP8266MOD)an das System. Arduino ist eine aus Software und hardware bestehende Physical-Computing-Plattform, dessen hardware aus einem Microcontrolle und analogen sowie digitalen Ein- und Ausgängen besteht. Ziel des Laborversuchs war es zunächst, mithilfe des Arduinos eine Lampe an- und ausschalten zu können. Im nächsten Schritt wird der Versuch um den Anschluss eines Wassersensors sowie eines Servomotors ergänzt. Im letzten Schritt soll mithilfe von MQTT der Nachrichtenaustausch vollzogen werden. 

## Aufsetzen einer Entwicklungsumgebung

 Der Beginn mit der eigentlichen Entwicklung setzte das Aufsetzen einer Entwicklungsumgebung voraus. Mithilfe dieser Entwicklungsumgebung können C-Code Programme geschrieben und auf den Arduino ausgeführt werden. Die hierfür notwendigen Schritte sind den nächsten Ausführungen zu entnehmen:

 1. Installation von Visual Studio Code
    
   Im ersten Schritt wird Visual Studio Code und anschließend im Pluginmanager das plugin "Plattform IO" installiert.

1. Treiberinstallation

    Das Aufspielen des C-Codes auf den Arduino erfolgt basierend auf einer Verbindung zwischen dem Entwicklungsrechner und dem Arduino. Die Verbindung wird mithilfe eines USB-Kabels umgesetzt. Zusätzlich wird eine manuelle Installation des Arduinotreibers vorausgesetzt, da der Entwicklungsrechner bei einer automatischen Installation des Treibers scheitert. 

2. Erstellung eines Beispielprojekts
   
    Nachdem der richtige Treiber installiert wurde, wird ein Beispielprojekt auf den Arduino aufgespielt. 

        /*
        * Blink
        * Turns on an LED on for one second,
        * then off for one second, repeatedly.
        */

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

    Das Beispielprojekt bringt die eingebaute LED des Arduino zum Blinken. Jede Sekunde wechselt es zwischen an und aus.
    
## Aufbau einer WLAN-Verbindung für den Arduino
   
 Um vom Arduino Nachrichten an den Raspbery schicken zu können, muss für den Arduino eine WLAN-Verbindung aufgebaut werden. Hierfür kann beispielsweise ein WiFi-Manager verwendet werden. Der Wifi-Manager agiert dabei folgendermaßen:
    
- Das Gerät erstellt ein Access Point (neues WLAN-Netz)
- Nach dem Verbinden mit dem WLAN wird man automatisch zu einer Website zum Einloggen weitergeleitet
- Das Gerät listet dort alle WLAN-Netze in der erreichbaren Umgebung auf
- Nun kann eine Verbindung mit dem gewünschten Netzwerk aufgebaut werden
- Handelt es sich beim gewünschten Netzwerk um ein gesichertes Netz, so ist das entsprechende Sicherheitspasswort auf der Weboberfäche zu übergeben
- Das Gerät verbindet sich anschließend mit dem WLAN und speichert die credentials in einem secure storage
- Das Gerät verbindet sich nun immer automatisch mit diesem WLAN
- Ein möglicher Wechsel zu einem anderen Netzwerk setzt das Löschen des internen Speichers voraus
    
Die konfigurierte WLAN-Verbindung ermöglicht nun, dass Nachrichten an den MQTT-Broker des Raspberies gesendet werden können.  

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

## Anbindung des Wassersensors und Motors?

Nun sollen die Daten des Wasserseonsor verarbeitet werden. Im ersten Schritt soll ein Motor angesteuert werden, der sich um 90 Grad dreht, sobald der Wassersensor sich im Wasser befindet. Ansonten soll der Motor zurück auf die Ausgangssituation gehen. 

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


## MQTT anbinden

 Anstatt nur den Motor zu schalten, sollen die Daten des Wassersensors nun via MQTT an den Raspbery übertragen werden. Dazu muss der Arduino die gesammelten Daten verarbeiten und an den Broker senden. Dieser Anwendungsfall kann mithilfe des folgenden Programms umgesetzt werden:

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

Die neuen Sensordaten werden alle 5 Sekunden an MQTT geschickt. Ebenso werden alle 5 Sekunden die neuen Nachrichten überprüft. Wenn neue Nachrichten vorhanden sind, werden diese ausgegeben. Wenn das erste Zeichen der empfangenen Nachricht eine 1 ist, wird die eingebaute LED des Arduinos eingeschaltet. Andererseits wird diese ausgeschaltet. Um eine dauerhafte Verbindung mit MQTT zu haben, wird allle 5 Sekunden überprüft ob eine Verbindung zum MQTT-Server vorhanden ist. Wenn dies nicht der Fall ist, wird versucht eine neue Verbindung aufzubauen. Um das Programm nutzen zu können, müssen die Nutzerdaten des WLANs in der Setup-Method eingetragen werden. Diese wurden aus Sicherheitsgründen in diesem Beispiel entfernt. 

# Tag 2 OpenHab Integration
   
Im nächsten Schritt wird der Laborversuch um die Software „OpenHab“ ergänzt, mit dessen Hilfe die Realisierung einer  Homeautomatisierung ermöglicht wird. Dies erfolgt beispielsweise dadurch, dass eine Vielzahl von Bibliotheken mitgelie-fert wird. Die Architektur von OpenHab weist einen skalierfähigen Charakter auf und lässt sich mit einem Baukasten vergleichen, da unterschiedliche Technologien und Systeme an die Architektur angebunden werden können. 

## Installation

Für die Installation bietet OpenHab eine [Installationsanleitung auf ihrer Website](https://www.openhab.org/download/) an. Im Rahmen der Installation wird zunächst der Repository Key hinzugefügt und auf dem Raspberry der folgende Befehl ausgeführt.

    wget -qO - 'https://bintray.com/user/downloadSubjectPublicKey?username=openhab' | sudo apt-key add

Im nächsten Schritt wurd für das Advanced Packacking Tool (APT) der HTTPS-transport aktiviert:

    sudo apt-get install apt-transport-https

Zudem wird auf dem Raspberry das OpenHab Repository hinzugefügt:

    echo 'deb https://dl.bintray.com/openhab/apt-repo2 stable main' | sudo tee /etc/apt/sources.list.d/openhab2.list

Anschließend erfolgt eine Aktualisierung des APT-Pakets auf dem aktuellsten Stand und das OpenHab Distribution Paket wird installiert:

    sudo apt-get update && sudo apt-get install openhab2

Zusätzlich sind die OpenHab-AddOns zu installieren:

    sudo apt-get install openhab2-addons

Nachdem diese Schritte durchgeführt wurden, hat sich unter Zunahme [einer weiteren Installationsanleitung](https://www.openhab.org/docs/installation/linux.html) ergeben, dass die Installation einen installierten Java JDK 8 voraussetzt. Dementsprechend erfolgte die Installation des Java JDK 8s über folgende Kommandozeile:

    sudo apt-get update && sudo apt-get install open-java8-jdk

Allerdings scheiterte die Installation des OpenHab-Pakets erneut, da das Paket als eine unsichere Paketinstallation angesehen wird. Dementsprechend war das Hin-zufügen des Repositories mit dem Zusatz [trusted=yes] zu versehen. Auf dieser Weise wird allen Quellen in diesem Repository vertraut. Der mit diesem Zusatz angereicherte Befehl lässt sich wie folgt aufschlüsseln:

    echo 'deb [trusted=yes] https://dl.bintray.com/openhab/apt-repo2 stable main' | sudo tee /etc/apt/sources.list.d/openhab2.list

Anschließend kann die OpenHab-Seite über die Ip und den Port 8080 aufgerufen werden. Da der Port 8080 bereits von einer anderen Anwendung belegt war, musste der Port umgeändert werden. Die Änderung des Ports kann durch eine Anpassung der Konfigurationsdatei, die auf dem Verzeichnis etc/default abgelegt ist, erfolgen.


## Anbindung einer Hue Bridge und Hue Lampe

Im nächsten Abschnitt des Laborversuchs wurden den Studenten vom Dozenten mit OpenHab anzubindende Geräte vorgestellt. Für die Durchführung des hier dargestellten Laborversuchs, fiel die Wahl auf die Hue Bridge und die Hue Lam-pe. Die Hue Bridge verfügt über keine WLAN-Funktion, weshalb ein Ethernet Kabel an das Gerät angeschlossen wird. Anschließend kann die Anbindung der Hue Bridge mit OpenHab und der Steuerung der Hue Lampen erfolgen. Für die Anbindung der Hue Bridge muss im OpenHab die PaperUI aufgerufen werden und anschließend über den Reiter Add Ons das Binding „Hue Binding“ installiert werden. Die nachfolgende Abbildung visualisiert das Hinzufügen des Hue Bindings:
![Screenshot OpenHab Add-Ons Installation](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/AddHueBinding.png?raw=true "Installation eines Hue Bindings")

Anschließend werden unter dem Reiter „inbox“ alle für die Kopplung verfügbaren Geräte angezeigt:

![Screenshot OpenHab Inbox-Übersicht](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Inbox.png?raw=true "Übersicht der anzukoppelnden Geräte")

Alle verfügbaren Things stehen nach Betätigung der Häkchen über den Reiter „Configuration“ zur Konfiguration zur Verfügung:

![Screenshot OpenHab Configuration_Things-Übersicht](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Configuration_Things2.png?raw=true "Übersicht der konfigurierbaren Things")

Für die Anbindung und Steuerung des Things ist ein Channel und ein Item zu konfigurieren. Dies kann durch ein Doppelklick auf dem entsprechenden Thing erfolgen. In der aufgepoppten Channelkonfiguration ist anschließend ein Channelname, Label und der Typ des Channels zu definieren. Zur Verdeutlichung wird im Folgenden eine Beispiel Channelkonfiguration veranschaulicht:

![Screenshot OpenHab Channel-Konfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Link_Channel.png?raw=true "Beispielhafte Channel-Konfiguration")

Nach Betätigung des im rechten unteren Rand befindlichen „Link“-Buttons, steht das zu steuernde Thing im Reiter Control zur Steuerung zur Verfügung:

![Screenshot OpenHab Control-Übersicht](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Overview_Control.png?raw=true "Übersicht der steuerbaren Things")

Als nächstes sollte die Steuerung der Lampe über das Kommunikationsprotokoll „MQTT“ umgesetzt werden. Ziel war es, die Lampe, abhängig vom ermittelten Feuchtigkeitswert durch den Wassersensor, zu dimmen.
Für die Kommunikation über MQTT kann das auf OpenHab verfügbare MQTT-Binding genutzt werden. Die Installation wird erneut über den Reiter „Add-Ons“ umgesetzt. Als nächstes ist unter dem Reiter „Inbox“ ein neuer Eintrag zu erstel-len, der zu folgenden Auswahlmöglichkeiten führt:

![Screenshot OpenHab MQTT Thing Binding-Konfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Options_MQTT_ThingBinding.png?raw=true "Konfigurationsmöglichkeiten vom MQTT-Binding")

Für den oben genannten Anwendungsfall werden lediglich ein MQTT Broker und ein Generic MQTT Thing benötigt. Dementsprechend wird als erstes ein MQTT Broker erstellt. Dem Broker ist ein Name und die IP-Adresse des Raspberry Pis zu übergeben. Als Port wird die 1883 gewählt. Die hier beschriebenen Brokereinstel-lungen sind der nächsten Abbildung zu entnehmen:

![Screenshot OpenHab MQTT Broker-Konfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Configuration_MQTT_Broker.png?raw=true "Konfiguration eines MQTT-Brokers")

Nachdem der MQTT Broker konfiguriert ist, ist anschließend noch ein Generic MQTT Thing zu erstellen. Diesem Thing ist anschließend ein Name zu übergeben und einem Broker zuzuweisen. Nach diesen beiden Schritten, können dem neu angelegten MQTT Thing mehrere Channels zugewiesen werden. Für diesen Chan-nel werden folgende Angaben vergeben:

    •	Channel Typ: Erwartetes Rückgabeformat (beispielsweise text value)
    •	Channel ID: Gibt an unter welchem Namen  
    •	MQTT Topic: Als Topic wird der beim Auslesen des Wassersensors ver-wendete topic „/water/out“ gewählt

Um nun die Lampenbeleuchtung über MQTT zu steuern, muss eine Regel unter etc/openhab2/rules/default.rules erstellt und beispielsweise mit dem folgenden Code versehen werden:

    rule "Water_Lamp_on"
    when
            Item GernericMQTTThing_ changed
    then
            if(GenericMQTTThing_.state > 100){
                    Lamp1.sendCommand(ON)
            }
            if(GenericMQTTThing_.state <= 100){
                    Lamp1.sendCommand(OFF)
            }
    end

Hierbei wird definiert, dass sobald der Wassersensor einen Wert über 100 an das MQTT Thing übergibt, die Lampe eingeschaltet wird. Bei einem darunter liegen-dem Wert wird die Lampe ausgeschaltet. Der Name des zu steuernden Things, kann beispielsweise über den Aufruf der Openhab-log ermittelt werden:

    Openhab-cli showlogs
#TO Change
Im nächsten Schritt wurde der Laborversuch dahingehend ergänzt, dass eine dimmbare Lampe angebunden wurde. Abhängig von dem ausgegebenen Was-sersensor-Wert sollte die Lampenhelligkeit um einen entsprechenden prozentualen Wert verringert oder gesteigert werden. Eine zusätzliche Bedingung ist, dass ein autorisierter Benutzer diesen Vorgang auslöst. Dieser Anwendungsfall lässt sich mit einer spezifischen ID einer NFC-Karte umsetzen. Für die Implementierung diesen Anwendungsfall musste die Regel folgendermaßen erweitert werden:

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

Die folgende Abbildung zeigt die prozentuale Änderung der Lampenbeleuchtung an. Für diesen Versuch wurde der Wassersensor im Wechsel befeuchtet und ge-trocknet. Zusätzlich wird die Beleuchtungsänderung nur geändert, sofern die rich-tige NFC-Karte an den NFC-Lesegeräte gehalten wurde:

![Screenshot OpenHab MQTT Broker-Konfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/OpenHab/Configuration_MQTT_Broker.png?raw=true "Konfiguration eines MQTT-Brokers")

# Tag 3 Blockchain Integration

Nachdem in den vorherigen Teilen verschiedene Sensoren mittels MQTT miteinander verbunden werden konnten, soll nun eine Blockchain Integration durchgeführt werden. Ziel dieses Teils ist die Einbindung von Smart-Contracts zur Schaltung von Sensoren. In unserem Beispiel soll ein Smart-Contract entwickelt werden, der dem Nutzer, welche die Lampe abhängig vom Wassersensor schalten möchte, ein bestimmter Geldbetrag in Rechnug gestellt werden. 
Um diese Scenario umsetzen zu können sollen die Smart-Contracts von Etherium eingesetzt werden. Sobald ein Nutzer seine NFC-Karte auf den Sensor legt, soll geschaut werden, ob der Nutzer noch genung Guthaben hat. Wenn dies der Fall ist soll im das Geld abgebucht und auf ein anderes Konto (dem Betreiber) zugeschrieben werden. Danach soll die Lampe geschaltet werden. 

## Installation

Um eine eigene Blockchain zum Testen bereitstellen zu können, soll Truffle eingesetzt werden. Truffle hilft den Entwicklern auf einfache Art und Weise Smart-Contracts for Etherium zu erstellen und zu testen. Die Installation von Truffle und Co. findet diesmal jedoch nicht auf dem Raspberry statt, sondern wird in unserem Fall auf einem seperaten Rechner bereitgestellt. 

### Node.js installiern

Bevor mit der Installation von Truffle begonnen werden kann, muss jedoch Node.js installiert werden. Dazu laden wir die neuste Version von [Node.js.](https://nodejs.org/download) herunter und installieren diese. 

### Truffle installieren

Als nächstes muss [Truffle](https://www.trufflesuite.com/) installiert werden. Dazu führen wir folgenden Befehl über NPM aus:

    npm install truffle -g

### Ganache installieren

Um eine einfache Oberfläche für die Blockchain zu erhalten nutzen wir ein weiters Tool mit dem Namen [Ganache](https://www.trufflesuite.com/ganache). Diese laden wir uns von der Webseite herunter und installieren es. 

## Einrichten

Als nächstes muss die Software eingerichtet werden. Als erstes öffnen wir dazu die zuvor installierte Applikation Ganache. Dort erstellen wir einen neuen Workspace und verbinden uns mit dem Server (in unserem Fall lokal, die Blockchain kann aber auch auf einem Seperaten Server installiert werden). 

![Screenshot Ganache Serverkonfiguration](https://github.com/fhoehn/iot-labor/blob/master/images/ganache/ganacheServerConfiguration.PNG?raw=true "Serverkonfiguration")

Nach dem Speichern des Workspaces öffnet sich eine Liste der aktuellen Accounts, wie sie in der Blockchain momentan hinterlegt sind. 

![Screenshot Accounts in Ganache](https://github.com/fhoehn/iot-labor/blob/master/images/ganache/ganacheListWithAccounts.PNG?raw=true "Liste der Accounts mit ihrem aktuellen Guthaben")


## Smart-Contract implementieren

Hier ein Beispiel wie wir einen Smart-Contract implementiert haben

## Blockchain in IOT-Netz einbinden

Hier dann noch Code wie man aus dem Python Programm auf die Blockchain Zugreift


