# Laborbericht 
Laborbericht für das Mastermodul 'Internet of Things'

# Kurzzusammenfassung

# Einführung

## Aufgabenstellung

## Eingesetzte Hardware

## Eingesetzte Software

# Tag 1

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