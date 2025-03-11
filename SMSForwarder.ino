#include <SoftwareSerial.h>

// Define SoftwareSerial pins for SIM800L
SoftwareSerial sim800l(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);   // Monitor output baud rate
  sim800l.begin(9600);  // SIM800L baud rate

  Serial.println("Initializing SIM800L...");
  delay(1000);

  // Check if SIM800L is responding
  sendCommand("AT");

  // Set fixed baud rate (optional)
  sendCommand("AT+IPR=9600");

  // Check Signal Strength
  Serial.println("Checking Signal Strength...");
  sendCommand("AT+CSQ");

  // Set SMS Center Number (Change this to your carrier's SMSC number)
  Serial.println("Setting SMS Center Number...");
  sendCommand("AT+CSCA=\"+981234567890\""); // Replace with your SMSC number

  // Enable SMS Text Mode
  Serial.println("Setting SMS Text Mode...");
  sendCommand("AT+CMGF=1");

  // Enable SMS Delivery Reports
  Serial.println("Enabling SMS Delivery Reports...");
  sendCommand("AT+CSMP=17,167,0,0");
  sendCommand("AT+CNMI=2,1,0,0,0");

  // Send SMS
  Serial.println("Sending SMS...");
  sendSMS("+989123456789", "Hello! This is a test message from SIM800L."); // Replace with your number

  Serial.println("Setup Complete!");
}

void loop() {
}

// Function to send AT commands and display response
void sendCommand(String command) {
  sim800l.println(command);
  delay(1000);
  while (sim800l.available()) {
    Serial.write(sim800l.read());
  }
  Serial.println();
}

// Function to send an SMS
void sendSMS(String phoneNumber, String message) {
  sim800l.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);
  sim800l.print(message);
  delay(500);
  sim800l.write(26);  // CTRL+Z to send
  delay(5000);

  Serial.println("SMS Sent!");
}
