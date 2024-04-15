#include <MKRNB.h>

#include "arduino_secrets.h"
const char PINNUMBER[] = SECRET_PINNUMBER;

NB nbAccess(true);     // Enable debugging
NBScanner scannerNetworks;
NBModem modemTest;

String IMEI = "";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB only.
  }

  Serial.println("NB IoT/LTE Cat M1 networks scanner");

  if (!modemTest.begin()) {
    Serial.println("Failed to initialize the modem. Check power and antenna.");
    while (true);
  }

  boolean connected = false;
  while (!connected) {
    if (nbAccess.begin(PINNUMBER) == NB_READY) {
      connected = true;
      Serial.println("Connected to the network");
    } else {
      Serial.println("Not connected. Trying again...");
      delay(1000);
    }
  }

  IMEI = modemTest.getIMEI();
  IMEI.replace("\n", "");
  if (IMEI != "") {
    Serial.print("Modem IMEI: ");
    Serial.println(IMEI);
  } else {
    Serial.println("Failed to retrieve IMEI.");
  }
}

void loop() {
  Serial.print("Current carrier: ");
  Serial.println(scannerNetworks.getCurrentCarrier());

  Serial.print("Signal Strength: ");
  Serial.print(scannerNetworks.getSignalStrength());
  Serial.println(" [0-31]");

  Serial.println("Scanning available networks. May take some seconds.");
  Serial.println(scannerNetworks.readNetworks());

  delay(10000);
}
