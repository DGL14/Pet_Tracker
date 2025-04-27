#include <TinyGPS++.h>          // Library for parsing GPS data
#include <SoftwareSerial.h>     // Library for software serial communication

// TinyGPS++ object for GPS data processing
TinyGPSPlus gps;

// Software serial for GPS module (RX pin 9, TX pin 8)
SoftwareSerial gpsSerial(9, 8);

// Software serial for SIM module (RX pin 3, TX pin 2)
SoftwareSerial simSerial(2, 3);

// Phone number to send SMS to (include country code)
const String phoneNumber = "+XXXXXXXXXXXX";

// Variables for storing location data
float latitude, longitude;
String incomingMessage = "";     // Buffer for incoming SMS messages
boolean messageReady = false;    // Flag indicating a complete message is received
// Sample coordinates: 20.724476, -103.328722

void setup() {
  // Initialize Arduino's hardware serial (for debugging)
  Serial.begin(9600);
  
  // Initialize GPS serial communication
  gpsSerial.begin(9600);
  
  // Initialize SIM module serial communication
  simSerial.begin(9600);

  Serial.println("Initializing GPS + SIM system...");
  delay(2000);  // Wait for modules to power up
  
  // Configure SIM module
  Serial.println("Configuring SIM module...");
  simSerial.println("AT");       // Basic AT command to check communication
  updateSimSerial();
  
  simSerial.println("AT+CMGF=1"); // Set SMS text mode (1 = text, 0 = PDU)
  updateSimSerial();
  
  // Configure SMS notification settings
  // Parameters: 1,2,0,0,0 = Show new SMS directly on serial line
  simSerial.println("AT+CNMI=1,2,0,0,0");
  updateSimSerial();
  
  Serial.println("System ready. Waiting for '1' message to send location...");
}

void loop() {
  // Continuously read GPS data with 1 second interval
  readGPS(1000);
  
  // Check for incoming SMS messages
  checkIncomingSMS();
  
  // Process message if one is ready
  if (messageReady) {
    // Check if message contains "1" (the location request command)
    if (incomingMessage.indexOf("1") >= 0) {
      Serial.println("Command '1' received. Sending location...");
      
      // Check if GPS location is valid
      if (gps.location.isValid()) {
        // Store current location
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        
        // Print location to serial monitor
        Serial.println("GPS location obtained:");
        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
        
        // Send location via SMS
        sendSMS();
      } else {
        // If no valid GPS fix, send error message
        Serial.println("Could not obtain valid GPS location.");
        sendErrorSMS();
      }
    }
    
    // Reset message flags
    messageReady = false;
    incomingMessage = "";
  }
}

// Function to read GPS data for specified duration
static void readGPS(unsigned long ms) {
  unsigned long start = millis();
  do {
    // Feed GPS data to the parser while available
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

// Function to check for incoming SMS messages
void checkIncomingSMS() {
  while (simSerial.available()) {
    char c = simSerial.read();
    
    // Echo to serial monitor for debugging
    Serial.write(c);
    
    // Append to message buffer
    incomingMessage += c;
    
    // Check for SMS indicator ("+CMT")
    if (incomingMessage.indexOf("+CMT") >= 0) {
      messageReady = true;  // Set flag when complete message is received
    }
  }
}

// Function to send SMS with current location
void sendSMS() {
  // Create Google Maps link with current coordinates
  String message = "My current location: ";
  message += "https://maps.google.com/?q=";
  message += String(latitude, 6);  // Latitude with 6 decimal places
  message += ",";
  message += String(longitude, 6); // Longitude with 6 decimal places
  
  // Send AT commands to prepare SMS
  simSerial.println("AT+CMGF=1");  // Ensure text mode
  updateSimSerial();
  
  // Set recipient phone number
  simSerial.print("AT+CMGS=\"");
  simSerial.print(phoneNumber);
  simSerial.println("\"");
  updateSimSerial();
  
  // Send message content
  simSerial.print(message);
  updateSimSerial();
  
  simSerial.write(26); // Send CTRL+Z to indicate end of message
  updateSimSerial();
  
  Serial.println("SMS sent: " + message);
}

// Function to send error SMS when no GPS fix is available
void sendErrorSMS() {
  // Create error message
  String message = "Could not get GPS location. Please ensure GPS module has clear sky view.";
  
  // Send AT commands to prepare SMS
  simSerial.println("AT+CMGF=1");
  updateSimSerial();
  
  // Set recipient phone number
  simSerial.print("AT+CMGS=\"");
  simSerial.print(phoneNumber);
  simSerial.println("\"");
  updateSimSerial();
  
  // Send error message
  simSerial.print(message);
  updateSimSerial();
  
  simSerial.write(26); // Send CTRL+Z to indicate end of message
  updateSimSerial();
  
  Serial.println("Error SMS sent: " + message);
}

// Function to handle two-way communication with SIM module
void updateSimSerial() {
  delay(500);  // Short delay for stable communication
  // Forward data from computer to SIM module
  while (Serial.available()) {
    simSerial.write(Serial.read());
  }
  // Forward data from SIM module to computer
  while (simSerial.available()) {
    Serial.write(simSerial.read());
  }
}