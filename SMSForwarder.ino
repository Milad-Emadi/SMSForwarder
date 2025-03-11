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
  if (sim800l.available()) {
    Serial.println(sim800l.readString());
  }

  // Set SMS Text Mode
  sim800l.println("AT+CMGF=1");
  delay(1000);
  Serial.println("Set SMS Text Mode");

  // Set recipient phone number
  sim800l.println("AT+CMGS=\"+989123456789\""); // Replace with actual phone number
  delay(1000);
  
  // Send message text
  sim800l.print("Hello! This is a test message from SIM800L.");
  delay(500);

  // Send CTRL+Z to finish SMS (ASCII code 26)
  sim800l.write(26);
  delay(5000);

  Serial.println("SMS Sent!");
}

void loop() {
}
