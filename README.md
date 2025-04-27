# Pet_Tracker
GPS tracker for pets via SMS message

Use the TEST_GPS and TEST_SIM800L codes to test the correct connection and operation of the modules.

## Hardware Connections

Make sure you connect the modules correctly:

- **NEO-6M GPS Module**:

- GPS TX → Arduino Pin 9
- GPS RX → Arduino Pin 8
- VCC → 3.3V or 5V (depending on module specifications)
- GND → GND

- **SIM Module (GSM)**:

- SIM TX → Arduino Pin 2
- SIM RX → Arduino Pin 3
- VCC → Typically 4V-5V (check specifications)
- GND → GND

  Once you check the correct operation of the modules, run the main code.

  ## Instructions for Use

1. Send an SMS with the number "1" to the SIM card number inserted in the SIM800L module.
2. The system will detect this message and respond with an SMS containing the current GPS location.
3. If the GPS cannot obtain a valid location (for example, if you are indoors), you will receive an error message.

## Important Notes

1. Make sure the SIM card has balance and is activated.
2. The SIM module must have a good signal to receive and send messages.
3. The GPS must be in a location with good visibility to the sky to obtain a signal.
4. The main code and TEST_SIM800L require a cell phone number (with country code) to send the SMS message to that phone.


