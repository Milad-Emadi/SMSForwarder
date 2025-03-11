#include <SoftwareSerial.h>

// Define SoftwareSerial pins for SIM800L
SoftwareSerial sim800l(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);   // Monitor output baud rate
  sim800l.begin(9600);  // SIM800L baud rate

  Serial.println("Initializing SIM800L...");
  delay(2000);

  // Check if SIM800L is responding
  if (!sendCommand("AT", 500, true)) {
    Serial.println("SIM800L not responding! Check wiring.");
    while (1);
  }

  // Set fixed baud rate (optional)
  sendCommand("AT+IPR=9600", 500, true);

  // Check Signal Strength
  Serial.println("Checking Signal Strength...");
  if (!checkSignalStrength()) {
    Serial.println("⚠️ Weak Signal! Try moving to a better location.");
    while (1); // Stop execution if no signal
  }

  // Check if registered on the network
  if (!checkNetworkRegistration()) {
    Serial.println("⚠️ Not registered on the network! Trying to reconnect...");
    sendCommand("AT+COPS=0", 5000, true);  // Auto-select best network
    delay(5000);
  }

  // Set SMS Center Number (Change this to your carrier's SMSC number)
  Serial.println("Setting SMS Center Number...");
  sendCommand("AT+CSCA=\"+989350001400\"", 500, true); // Replace with your SMSC number

  // Enable SMS Text Mode
  Serial.println("Setting SMS Text Mode...");
  sendCommand("AT+CMGF=1", 500, true);

  // Enable SMS Delivery Reports
  Serial.println("Enabling SMS Delivery Reports...");
  sendCommand("AT+CSMP=17,167,0,0", 500, true);
  sendCommand("AT+CNMI=2,1,0,0,0", 500, true);

  // Send SMS
  Serial.println("Sending SMS...");
  if (sendSMS("+989123456789", "Hello! This is a test message from SIM800L.")) {  // Replace with your number
    Serial.println("✅ SMS Sent Successfully!");
  } else {
    Serial.println("❌ SMS Failed! Check SIM balance & network.");
  }

  Serial.println("Setup Complete!");
}

void loop() {
}

// Function to send AT commands and display response
bool sendCommand(String command, int timeout, bool debug) {
  sim800l.println(command);
  delay(timeout);

  if (sim800l.available()) {
    String response = sim800l.readString();
    if (debug) {
      Serial.println(response);
    }
    return true;
  }
  return false;
}

// Function to check signal strength
bool checkSignalStrength() {
  sim800l.println("AT+CSQ");
  delay(1000);

  if (sim800l.available()) {
    String response = sim800l.readString();
    Serial.println(response);

    int csqIndex = response.indexOf("+CSQ: ");
    if (csqIndex != -1) {
      int signalStrength = response.substring(csqIndex + 6, response.indexOf(",", csqIndex)).toInt();
      Serial.print("📡 Signal Strength: ");
      Serial.println(signalStrength);

      if (signalStrength > 5) {  // Anything below 5 is too weak
        return true;
      }
    }
  }
  return false;
}

// Function to check network registration
bool checkNetworkRegistration() {
  sim800l.println("AT+CREG?");
  delay(1000);

  if (sim800l.available()) {
    String response = sim800l.readString();
    Serial.println(response);

    if (response.indexOf("+CREG: 0,1") != -1 || response.indexOf("+CREG: 0,5") != -1) {
      Serial.println("✅ Registered on Network!");
      return true;
    }
  }
  return false;
}

// Function to send an SMS
bool sendSMS(String phoneNumber, String message) {
  sim800l.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);

  sim800l.print(message);
  delay(500);

  sim800l.write(26);  // CTRL+Z to send
  delay(5000);

  if (sim800l.available()) {
    String response = sim800l.readString();
    Serial.println(response);

    if (response.indexOf("OK") != -1) {
      return true;
    }
  }
  return false;
}
