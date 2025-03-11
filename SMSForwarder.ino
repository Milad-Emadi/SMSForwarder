#include <SoftwareSerial.h>

SoftwareSerial sim800l(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);

  Serial.println("Initializing SIM800L...");
  delay(1000);

  // Check module response
  sim800l.println("AT");
  delay(1000);
  Serial.println(sim800l.readString());

  // Check Signal Strength
  sim800l.println("AT+CSQ");
  delay(1000);
  Serial.println("Signal Strength: ");
  Serial.println(sim800l.readString());

  // Set SMSC Number (Change to your carrier's SMSC)
  sim800l.println("AT+CSCA=\"+981234567890\"");
  delay(1000);
  Serial.println("Set SMS Center");

  // Enable SMS delivery reports
  sim800l.println("AT+CSMP=17,167,0,0");
  delay(1000);
  sim800l.println("AT+CNMI=2,1,0,0,0");
  delay(1000);
  Serial.println("SMS Delivery Report Enabled");

  // Set SMS Text Mode
  sim800l.println("AT+CMGF=1");
  delay(1000);
  Serial.println("Set SMS Text Mode");

  // Send SMS
  sim800l.println("AT+CMGS=\"+989123456789\""); // Replace with actual phone number
  delay(1000);
  sim800l.print("Hello! This is a test message from SIM800L.");
  delay(500);
  sim800l.write(26); // CTRL+Z to send
  delay(5000);
  
  Serial.println("SMS Sent!");
}

void loop() {
}
