#include <MKRNB.h>
#include <MQTT.h>

NB nbAccess;
GPRS gprs;
MQTTClient client;

const char PINNUMBER[]     = ""; // PIN number if necessary
const char GPRS_APN[]      = ""; // GPRS APN
const char GPRS_LOGIN[]    = ""; // GPRS Login
const char GPRS_PASSWORD[] = ""; // GPRS Password
const char MQTT_BROKER[]   = ""; // MQTT Broker address
const int MQTT_PORT        = ;    // MQTT Broker port

const int BATTERY_PIN = A1; // Define the battery analog input pin

void setup() {
  // Begin serial communication
  Serial.begin(9600);

  // Connect to NB and GPRS
  nbAccess.begin(PINNUMBER);
  gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD);

  // Connect to MQTT Broker
  client.begin(MQTT_BROKER, MQTT_PORT, nbAccess);

  // Check for successful connection and reconnect if necessary
  while (!client.connect("arduino", "try", "try")) {
    delay(1000);
  }
}

void loop() {
  // Read the battery voltage
  int batteryAnalogValue = analogRead(BATTERY_PIN);
  float batteryVoltage = mapBatteryReadingToVoltage(batteryAnalogValue);

  // Publish the voltage to the MQTT topic
  client.publish("sensor/battery", String(batteryVoltage));

  // Call the loop function continuously to maintain the connection
  client.loop();

  // Sleep or delay if necessary
  delay(10000); // Delay for 10 seconds
}

float mapBatteryReadingToVoltage(int analogValue) {
  // Convert the analog reading to voltage
  // This depends on your specific battery and needs to be calibrated
  float voltage = (analogValue * (3.3 / 1023.0)) * (R1 + R2) / R2; // Example for a voltage divider
  return voltage;
}