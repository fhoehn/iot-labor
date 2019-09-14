# Laborbericht 
Laborbericht für das Mastermodul 'Internet of Things'

# Kurzzusammenfassung

# Einführung

## Aufgabenstellung

## Eingesetzte Hardware

## Eingesetzte Software

# Teil 1 - NFC und MQTT

Zu Beginn des Labors musste zuerst der Raspberry PI konfiguriert werden, damit mit dem Entwickeln der Scenarios begonnen werden konnte. 
Dazu muss der Rapsberry mit dem WLAN verbunden werden und um später mit Hilfe des PuTTY darauf zugreifen zu können muss SSH aktiviert wreden. 

## Raspberry einrichten

1. WLAN konfigurieren
    1. Ein Bildschirm eun eine Tastatur werden benötigt um die ersten Schritte machen zu könnnen
    2. Der Raspberry muss via Ethernet an das Netzwerk angeschlossen werden.        Dazu muss die Konfigurationsdatei geöffnet werden. 
   
            sudo nano /etc/wpa_supplicant/wpa_supplicant.conf 
        In diese muss das Netzwerk und der Schlüssel hnzugefügt werden.
        
            network={
                ssid="WiLaWiFi063C"
                psk="p7-15Dex9.4A"
            }
        
    3. Danach muss das Netzwerkmodul neugestartet werden.
            
            sudo ifdown wlan0 && duwo ifup wlan0
2. SSH konfigurieren
    1. TODO SSH konfigurieren

3. Verbinden via PuTTY

    Nun kann sich mit dem PuTTY zum Raspberry verbunden werden und somit per Remote weiterentwickelt werden. Dafür werden der Nutzername und das Passwort benötigt. 

4. Python installieren
    
    Für die weitere Entwicklung werden nun sowohl Python als auch nodered installiert. Außerdem werden zuvor die aktuellen Packete aktualisiert.

        sudo apt-get update && sudo apt-get install python3 python3-pip cmake nodered

5. NFC-Pakete installieren
   
   Um auf die NFC-Schnittstelle zugreifen zu können werden ein paar weitere Pakete benötigt. Dazu muss zuerst ein ZIP mit diesen Paketen auf den Raspberry geladen werden.

        cd /tmp ; wget https://farberg.de/talks/iot/downloads/nfc_debian_packages.zip
    
    Danach muss dieses entpackt nud installiert werden

        cd /tmp
        unzip nfc_debian_packages.zip
        cd debian_packages 
        sudo dpkg --install *.deb 
        sudo apt-get --fix-broken install

6. MQTT Client installieren

    Um später mit dem Raspberry MQTT-Nachrichten verschicken und empfangen zu können, wird ein MQTT-Client benötigt. Dazu installieren wir den 'pika' Client.

        sudo pip3 install nxppy paho-mqtt pika
    
7. Serial Peripheral Interface (SPI) konfigurieren

    Falls dies noch nicht konfiguriert wurde muss dies nun getan werden. In unserem Fall war dies schon konfiguriert. 

## NFC einlesen

Nachdem der Raspberry konfiguriert wurde, konnte nun damit begonnen werden ein kleines Programm zum einlesen von NFC-Chips zu erstellen.

1. nxppy-Library installieren
    
    Die nxppy-Library wird zum einfachen Einlesen und Schreiben von NFC-Chips benötigt. Diese musste zuerst installiert werden. 

        pip install nxppy

2. UID von NFC ausgeben
   
   Mit Hilfe eines kleinen Python-Programms kann die UID eines NFC-Chips eingelesen und auf der Konsole ausgegeben werden. 

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
        
    Nach dem Starten des Programms liest es nun jede Millisekunde den aktuellen Wert des NFC-Lesegerätes ein und gibt diesen auf der Konsole aus. Wenn verschiedene NFC-Chips auf das Lesegerät gehalten werden, werden nun die IDs dieser Chips auf der Konsole ausgegeben. 

## UIDs an MQTT-Broker senden

Die UIDs der einzelnen NFC-Chips können nun per Python programm eingelesen werden. Die nächste Aufgabe ist das Anbinden eines MQTT-Brokers. Die eingelesenen UIDs sollen an den MQTT-Broker gesendet werden. Dazu wird ein Node-RED Server verwendet. [Mehr Details über Node-RED finden sie hier](https://nodered.org/)

1. Node-RED installieren
   
   Zuerst muss der Node-RED MQTT-Broker installiert werden. Dazu nutzen wir 'npm'.

        npm install node-red-contrib-mqtt-broker

2. Node-RED starten
   
   Nach dem erfolgreichen installieren von Node-RED muss dies nun gestartet werden. 

        node-red-start

    Danach kann die Node-RED Weboberfläche unter `https://<ip>:1880` erreicht werden. Die IP des Raspberry, welche auch zum verbinden via PuTTY genutzt wird, muss dazu verwendet werden. 

3. Flow erstellen

    Als nächstes sollte ein Flow erstellt werden, welcher Nachrichten vom MQTT-Broker entgegen nimmt nd im Node-RED als DEBUG-Information ausgibt. 
    
    1. Wir erstellen einen Flow, der eine MQTT-Eingabe hat, die eine Verbindungen zu einer Debug-Ausgabe besitzt. Dazu müssen die zwei Bauteile per Drag and Drop von der Toolbox eingefügt und miteinander vernunden werden. 
    
    #TODO[Add Image here]

    2. Danach muss der MQTT-Server eingestellt werden. Dies kann in den Eigenschaften des Inputs getan werden. 
   
    #TODO[Add Image here]

    3. Das Topic der Eingabe muss ebenfalls festgelegt werden. Dazu tragen wir in den Eigenschaften der der MQTT-Eingabe das Topic "/nfc/1" ein.
   
    #TODO[Add Image here]

    4. Danach muss der Flow nur noch deployed werden.

    #TODO[Add Image here]

4. Daten an MQTT senden

    Nach dem erfolgreichen Erstellen des Flows zum Verarbeiten der MQTT-Nachrichten sollen nun die Daten von der NFC-Chips an die Queue geschickt werden. Dies geschieht wiederum in einem Python Programm. Dafür legen wir eine neue Datei an und programmieren in dieser unser neues Programm.

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
        
    Diese Programm ist eine Erweiterung des letzten Programms. Anstelle die UIDs nur auf der Konsole auszugeben, werden die Daten nun auch and den MQTT-Broker gesendet. Diese Nachrichten können dann von Node-RED abgegriffen und unser Flow gestartet werden. Dazu muss das Programm nur noch gestartet und verschieden NFC-Chips auf das Lesegerät gelegt werden. 

5. DEBUG Informationen einsehen
   
   In Node-RED können wir nun die Nachrichten einsehen.

   #TODO[Add Image here]

## REST Service bereitstellen

Eine weiter Aufgabe wäre das bereitstellen eines REST Services für die letzten 10 eingelesen UIDs gewesen. Diese Aufgabe haben wir aus Zeitgründen nicht implementiert, wird jedoch für den weiteren Verlauf auch nicht benötigt. 

# Teil 2 - Arduino

Der zweite Teil des Labors befasste sich mit dem Anschließen eines Arduinos (ESP8266MOD) an das System. Zu Beginn sollte eine einfache Lampe mit dem Arduino an- und ausgeschalten werden können. Danach wurde sich damit befasst, einen Wassersensor und einen Servomotor anzuschließen. Diese solten dann über MQTT miteinander verbunden werden. 

## Entwicklungsumgebung aufsetzen
 Bevor mit der eigentlichen etnwicklung begonnen werden konnte, musste eine Entwicklungsumgebung aufgesetzt werden, die es uns ermöglicht C-Code für den Arduino zu entwicklen und diesen dann auch auf dem arduino laufen zu lassen. Dazu gingen wir wie folgt vor.

 1. Visual Studio Code installieren
    
    Als erstes installierten wir Visual Studio Code mit dem zusätzlichen Plugin "Plattform IO". Diese kann in Visual Studio Code über den Pluginmanager installiert werden. 

2. Treiber installieren

    Um den C-Code über ein USB-Kabel auf den Arduino aufspielen zu können, muss zuerst der Treiber dafür installiert werden. Da Windows in unserem Fall nicht automatisch den Treiber finden konnte, mussten wir diesen manuell herunterladen und dann installieren. 

3. Beispielprojekt
   
    Nach dem erfolgreichen einrichten sollte nun ein einfaches Beispielprojekt auf den Arduino aufgespielt werden. 

    #TODO[Add example project URL]

    #TODO[Add example project code]

    
4. Mit WiFi verbinden
   
    Um vom Arduino Nachrichten an den Raspbery schicken zu könne, wird eine WiFi-Verbindung benötigt. Dazu gibt es verschiedene Möglichkeiten. In diesem Fall nutzen wir einen WiFi-Manager um uns mit dem WiFi verbinden zu können. Der Wifi-Manager agiert dabei folgendermaßen:
    
    - Device erstellt Access Point (neues WLAN-Netz)
    - Nach dem Verbinden mit dem WLAN wird man automatisch eine Website zum Einloggen weitergeleitet
    - Das Gerät listet dort alle WLAN-Netze in der Umgebung auf
    - Nun kann man ein Netz auswählen, mit dem man sich verbinden möchte
    - Wenn es sich um ein gesichertes Netz handelt, muss das Password eingegebn werden
    - Das Gerät verbindet sich automatisch mit dem WLAN und speichert die Credentials in einem Secure Storage
    - Das Gerät verbindet sich nun immer automatisch mit diesem WLAN
    - Zum erneuten Eingeben der Daten muss der Speicher gelöscht werden
    
    Danach ist das Gerät mit dem WLAN verbunden und wir können nun damit fortfahren die Nachrichten an den MQTT-Broker des Raspberies zu senden.  

    #TODO[add Code]

5. Wassersensor anbinden

    Nun sollen die Daten des Wasserseonsor verarbeitet werden. Im ersten Schritt soll ein Motor angesteuert werden, der sich auf 90 Grad dreht, sobald der Wassersensor sich im Wasser befindet. Ansonten soll der Motor zurück auf die Ausgangssituation fahren. 

    #TODO[add Code]

6. MQTT anbinden

    Anstatt nur den Motor zu schalten, sollen die Daten des Wassersensors nun via MQTT an den Raspbery übertragen werden. Dazu muss der Arduino die gesammelten Daten verarbeiten und an den Broker senden. Darum kümmert sich folgendes Programm.

    #TODO[add Code]
   
