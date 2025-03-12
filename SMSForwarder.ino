#include <SoftwareSerial.h>

// Define SoftwareSerial pins for SIM800L
SoftwareSerial sim800l(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  sim800l.begin(115200);

  Serial.println("Initializing SIM800L...");
  delay(2000);

  if (!sendCommand("AT", 500, true)) {
    Serial.println("SIM800L not responding! Check wiring.");
    while (1);
  }

  // sendCommand("AT+IPR=9600", 500, true);

  Serial.println("Checking Signal Strength...");
  if (!checkSignalStrength()) {
    Serial.println("⚠️ Weak Signal! Try moving to a better location.");
    //while (1);
  }

  if (!checkNetworkRegistration()) {
    Serial.println("⚠️ Not registered on the network! Trying to reconnect...");
    sendCommand("AT+COPS=0", 5000, true);
    delay(5000);
  }

  Serial.println("Setting SMS Center Number...");
  sendCommand("AT+CSCA=\"+989350001400\"", 500, true);

  Serial.println("Setting SMS Text Mode...");
  sendCommand("AT+CMGF=1", 500, true);

  Serial.println("Enabling SMS Delivery Reports...");
  sendCommand("AT+CSMP=17,167,0,0", 500, true);
  sendCommand("AT+CNMI=2,1,0,0,0", 500, true);

  Serial.println("Activating GPRS...");
  if (!activateGPRS("mtnirancell")) {  // Replace with your APN
    Serial.println("❌ GPRS Activation Failed!");
    while (1);
  }

  Serial.println("Fetching Public IP...");
  String publicIP = getPublicIP();
  if (publicIP != "") {
    Serial.print("🌎 Your Public IP: ");
    Serial.println(publicIP);

    Serial.println("Sending SMS with IP...");
    if (sendSMS("+989128178658", "Your Public IP is: " + publicIP)) {  // Replace with your number
      Serial.println("✅ SMS Sent Successfully!");
    } else {
      Serial.println("❌ SMS Failed! Check SIM balance & network.");
    }
  } else {
    Serial.println("❌ Failed to get Public IP!");
  }

  Serial.println("Setup Complete!");
}

void loop() {}

// Function to send AT commands and check response
bool sendCommand(String command, int timeout, bool debug) {
  sim800l.println(command);
  delay(timeout);

  if (sim800l.available()) {
    String response = sim800l.readString();
    if (debug) Serial.println(response);
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

      return signalStrength > 5; 
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

    return response.indexOf("+CREG: 0,1") != -1 || response.indexOf("+CREG: 0,5") != -1;
  }
  return false;
}

// Function to activate GPRS
bool activateGPRS(String apn) {
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", 1000, true);
  sendCommand("AT+SAPBR=3,1,\"APN\",\"" + apn + "\"", 1000, true);
  sendCommand("AT+SAPBR=1,1", 5000, true);
  return sendCommand("AT+SAPBR=2,1", 1000, true);
}

// Function to get Public IP using HTTP GET request
String getPublicIP() {
  sendCommand("AT+HTTPINIT", 1000, true);
  sendCommand("AT+HTTPPARA=\"CID\",1", 500, true);
  sendCommand("AT+HTTPPARA=\"URL\",\"http://api64.ipify.org?format=json\"", 500, true);
  sendCommand("AT+HTTPACTION=0", 5000, true);  // Start GET request
  delay(5000);

  sim800l.println("AT+HTTPREAD");  // Read HTTP response
  delay(1000);

  String response = "";
  while (sim800l.available()) {
    response += char(sim800l.read());
  }
  sendCommand("AT+HTTPTERM", 500, true);  // Terminate HTTP service

  int start = response.indexOf("{\"ip\":\"") + 7;
  int end = response.indexOf("\"}", start);

  if (start > 7 && end > start) {
    return response.substring(start, end);
  }

  return "";
}

// Function to send an SMS
bool sendSMS(String phoneNumber, String message) {
  sim800l.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);

  sim800l.print(message);
  delay(500);

  sim800l.write(26);
  delay(5000);

  if (sim800l.available()) {
    String response = sim800l.readString();
    Serial.println(response);
    return response.indexOf("OK") != -1;
  }
  return false;
}
