#include <SoftwareSerial.h>      // Include the SoftwareSerial library for serial communication

// Create a SoftwareSerial object on pins 3 (RX) and 2 (TX) to communicate with the SIM800L module
SoftwareSerial mySerial(2, 3);  

void setup()
{
  // Initialize the hardware serial port (USB) for communication with the computer at 9600 baud
  Serial.begin(9600);
  
  // Initialize the software serial port for communication with the SIM800L module at 9600 baud
  mySerial.begin(9600);

  // Print initialization message to the serial monitor
  Serial.println("Initializing..."); 
  delay(1000);  // Wait for 1 second for module initialization

  // Send basic "AT" command to check if module is responding
  mySerial.println("AT");
  updateSerial();  // Check and display response

  // Set SMS text mode (1 = text mode, 0 = PDU mode)
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  
  // Set the recipient phone number for SMS (with country code)
  mySerial.println("AT+CMGS=\"+XXXXXXXXXXXX\""); // Replace with your phone number (include country code)
  updateSerial();
  
  // Send the SMS message content
  mySerial.print("Hello"); // Replace with your message text
  updateSerial();
  
  // Send Ctrl+Z (ASCII 26) to indicate end of message and send the SMS
  mySerial.write(26);
}

void loop()
{
  // Continuously check for serial communication updates
  updateSerial();
}

// Function to handle two-way communication between Arduino and SIM800L module
void updateSerial()
{
  delay(500);  // Short delay to allow data to arrive
  
  // Forward any data received from computer (Serial) to the SIM800L module (mySerial)
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  
  // Forward any data received from SIM800L module (mySerial) to the computer (Serial)
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}