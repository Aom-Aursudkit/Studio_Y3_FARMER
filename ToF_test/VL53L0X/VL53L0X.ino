#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <esp32-hal-ledc.h>

// Create an instance of the sensor object
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int pwmPin = 25;
const int pwmFreq = 100;
const int pwmResolution = 12;  // 12-bit => 0–4095

void setupPWM() {
  ledcAttach(pwmPin, pwmFreq, pwmResolution);
  ledcWrite(pwmPin, 0);
}

void outputDistancePWM(int mm) {
  mm = constrain(mm, 0, 2000);  // choose your max range

  int duty = map(mm, 0, 2000, 0, 4095);

  ledcWrite(pwmPin, duty);
}

void setup() {
  Serial.begin(115200);
  setupPWM();

  Serial.println(F("Adafruit VL53L0X Test"));

  // Initialize the sensor and check if it's found
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;  // Stop execution if the sensor isn't found
  }

  // Optional: Increase the sensor sensitivity and range (at the cost of speed/accuracy)
  // lox.setSignalRateLimit(0.1);
  // lox.setVcselPulsePeriod(VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
  // lox.setVcselPulsePeriod(VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
  // Increase timing budget (slower but more accurate)
  lox.setMeasurementTimingBudgetMicroSeconds(33000);  // Default is 20000

  // PRE-RANGE VCSEL pulse period
  lox.setVcselPulsePeriod(VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);

  // FINAL-RANGE VCSEL pulse period
  lox.setVcselPulsePeriod(VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);

  // Reduce false detections (stronger filter)
  lox.setLimitCheckEnable(VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
  lox.setLimitCheckValue(VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
                         (FixPoint1616_t)(0.1 * 65536));  // 0.1 MCPS

  Serial.println(F("VL53L0X Sensor Initialized!"));
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false);  // Pass 'false' for single polling mode

  if (measure.RangeStatus != 4) {  // Status 4 means "Range Invalid"
    int mm = measure.RangeMilliMeter;
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
    outputDistancePWM(mm);
  } else {
    Serial.println(" --- Out of range / Error ---");
    outputDistancePWM(0);
  }

  delay(50);
}