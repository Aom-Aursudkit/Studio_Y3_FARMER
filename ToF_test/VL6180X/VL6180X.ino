#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

void setup() {
  Serial.begin(115200);

  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
}

void loop() {
  float lux = vl.readLux(VL6180X_ALS_GAIN_5);

  Serial.print("Lux: "); Serial.println(lux);
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); Serial.println(range);
  }

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  delay(50);
}



// #include <Wire.h> // Required for I2C communication
// #include "Adafruit_VL6180X.h"

// // Create an instance of the sensor object using the default I2C address (0x29)
// Adafruit_VL6180X vl = Adafruit_VL6180X();

// void setup() {
//   // Initialize Serial communication at a baud rate of 115200
//   Serial.begin(115200);
  
//   // Optional: Wait for serial port to connect (useful for some boards like Leonardo/Micro)
//   // while (!Serial); 

//   Serial.println(F("Adafruit VL6180X Test!"));

//   // Initialize the sensor
//   if (! vl.begin()) {
//     Serial.println(F("Failed to find sensor! Check wiring."));
//     while (1); // Stop execution if the sensor isn't found
//   }
  
//   Serial.println(F("VL6180X Sensor found!"));
// }

// void loop() {
//   // Read the distance in millimeters
//   uint8_t range = vl.readRange();
//   uint8_t status = vl.readRangeStatus(); // Get the status of the reading
  
//   // Check the status to ensure a valid reading
//   if (status == VL6180X_ERROR_NONE) {
//     Serial.print("Range: ");
//     Serial.print(range);
//     Serial.println(" mm");
//   } else {
//     // Print an error message if the reading was not successful
//     Serial.print("Range Error Status: ");
//     Serial.println(status);
//     // You can add more detailed error handling here based on the status codes.
//   }
  
//   delay(500); // Wait half a second before taking the next measurement
// }




