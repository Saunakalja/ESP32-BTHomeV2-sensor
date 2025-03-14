#include <ArduinoBLE.h>

// https://bthome.io/format/
//                    |----Flags-----|  |-----------Service data---------------|  |------------------Complete local name-------------------------|
uint8_t rawData[] = { 0x02, 0x01, 0x06, 0x06, 0x16, 0xd2, 0xfc, 0x40, 0x21, 0x00, 0x0A, 0x09, 0x42, 0x54, 0x48, 0x6F, 0x6D, 0x65, 0x20, 0x76, 0x32 };
//                    |-L-|             |-L-|       |--UUID--| |-DI-| |---MD---|  |-L-|       |-------------------"BTHome v2"--------------------|

// |-L-|  = Advertising Data (AD) element length
// |-DI-| = BTHome Device Information
// |-MD-| = Measurement Data (0x21 = motion, 0 (False = Clear)/1 (True = Detected)

// Flags always the same I think.
// Service data edit DI and MD and calculate correct L.
// Local name use https://www.rapidtables.com/convert/number/ascii-to-hex.html to get values and calculate correct L.


void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1) {}
  }

  // Measure here, send data and go to sleep for battery operated devices
}

void send_bthome_data(void) {
    // Build advertising data packet
  BLEAdvertisingData advData;
  // If a packet has a raw data parameter, then all the other parameters of the packet will be ignored
  advData.setRawData(rawData, sizeof(rawData));
  // Copy set parameters in the actual advertising packet
  BLE.setAdvertisingData(advData);

  BLE.advertise();
  delay(200); // This delay is needed if we want to go to sleep after advertise. Not sure how long this should be?
  BLE.stopAdvertise(); // This currently simulates going to sleep.
}

void loop() {
  // Change motion data value: Clear/Detected
  if (rawData[9] == 0)
    rawData[9] = 1;
  else
    rawData[9] = 0;
  
  Serial.print(rawData[9]);

  send_bthome_data();
  delay(5000);
}
