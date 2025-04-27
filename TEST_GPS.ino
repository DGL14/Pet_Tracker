#include <TinyGPS++.h>          // Include the TinyGPS++ library for GPS data parsing
#include <SoftwareSerial.h>     // Include the SoftwareSerial library for serial communication

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
// Connect the GPS TX (transmit) pin to Arduino Digital 8
// Connect the GPS RX (receive) pin to Arduino Digital 9
SoftwareSerial ss(8, 9);        // Create a SoftwareSerial object on pins 9 (RX) and 8 (TX)

void setup() {
  // Start the Arduino hardware serial port at 9600 baud for communication with the computer
  Serial.begin(9600);
  
  // Start the software serial port at the GPS's default baud rate (9600)
  ss.begin(9600);

  // Print initialization messages
  Serial.println(F("NEO-6M GPS Module Test"));
  Serial.println(F("Waiting for GPS data..."));
}

void loop() {
  // This smart delay is needed for the TinyGPS++ library to parse data
  // It maintains GPS data flow while waiting
  smartDelay(1000);

  Serial.println();
  Serial.println(F("GPS Data:"));
  
  // Check if location data is valid and print it
  if (gps.location.isValid()) {
    Serial.print(F("Location: "));
    Serial.print(gps.location.lat(), 6);  // Print latitude with 6 decimal places
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6); // Print longitude with 6 decimal places
  } else {
    Serial.println(F("Location: Not Available"));
  }

  // Check if date data is valid and print it
  if (gps.date.isValid()) {
    Serial.print(F("Date: "));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.println(gps.date.year());
  } else {
    Serial.println(F("Date: Not Available"));
  }

  // Check if time data is valid and print it with leading zeros if needed
  if (gps.time.isValid()) {
    Serial.print(F("Time: "));
    if (gps.time.hour() < 10) Serial.print(F("0"));  // Add leading zero for hours < 10
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0")); // Add leading zero for minutes < 10
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0")); // Add leading zero for seconds < 10
    Serial.println(gps.time.second());
  } else {
    Serial.println(F("Time: Not Available"));
  }

  // Check if speed data is valid and print it
  if (gps.speed.isValid()) {
    Serial.print(F("Speed (knots): "));
    Serial.println(gps.speed.knots());
  } else {
    Serial.println(F("Speed: Not Available"));
  }

  // Check if altitude data is valid and print it
  if (gps.altitude.isValid()) {
    Serial.print(F("Altitude: "));
    Serial.print(gps.altitude.meters());
    Serial.println(F(" meters"));
  } else {
    Serial.println(F("Altitude: Not Available"));
  }

  // Print satellite count (value may be 0 if no fix)
  Serial.print(F("Satellites: "));
  Serial.println(gps.satellites.value());

  // Print HDOP (Horizontal Dilution of Precision) value
  Serial.print(F("HDOP: "));
  Serial.println(gps.hdop.value());
  
  Serial.println();
  Serial.println(F("Waiting for more data..."));
}

// Custom delay function that maintains GPS data flow while waiting
// It feeds the GPS parser with incoming data during the delay period
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    // While data is available, read it and feed it to the GPS parser
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);  // Continue until the specified time has elapsed
}