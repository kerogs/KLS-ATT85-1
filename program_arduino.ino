/*
 * ============================================================================
 * Project      : Electromagnetic Field Detector
 * Author       : Kerogs
 * Version      : 2.1-arduino
 * URL          : https://github.com/kerogs/KS-ATT85-1
 *
 * Description  :
 * This program measures variations in the analog input coming from a handmade
 * antenna — a copper wire loop designed to capture ambient electromagnetic fields.
 *
 * These variations are analyzed in real time to detect disturbances such as
 * atmospheric electrical activity (storms), human presence, or electromagnetic noise.
 *
 * When a significant deviation from the baseline threshold is detected, a LED lights up
 * and a buzzer sounds an alert.
 * ============================================================================
 */
#define SIGNAL_PIN A0
#define LED_PIN 9
#define BUZZER_PIN 8

int detectionThreshold = 300;          // Detection threshold level
int maxValue = 0;                      // Maximum value reached
unsigned long lastPeak = 0;            // Last time we beat the record
const unsigned long resetDelay = 5000; // 5 seconds (quick test)

// Filter parameters
const int FILTER_SIZE = 5;  // Number of samples for moving average
int buffer[FILTER_SIZE];
int filterIndex = 0;
long filterSum = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize buffer with the first reading
  int initialValue = analogRead(SIGNAL_PIN);
  for (int i = 0; i < FILTER_SIZE; i++)
  {
    buffer[i] = initialValue;
    filterSum += initialValue;
  }
}

void loop()
{
  int rawValue = analogRead(SIGNAL_PIN);

  // Update moving average buffer: remove oldest value and add new one
  filterSum -= buffer[filterIndex];
  buffer[filterIndex] = rawValue;
  filterSum += rawValue;

  filterIndex++;
  if (filterIndex >= FILTER_SIZE)
    filterIndex = 0;

  // Calculate filtered average
  int value = filterSum / FILTER_SIZE;

  // Serial Plotter output
  Serial.print("Value:");
  Serial.print(value);
  Serial.print(", Threshold:");
  Serial.print(detectionThreshold);
  Serial.print(", Max:");
  Serial.println(maxValue);

  // ✅ Check if value exceeds threshold (value > threshold + margin)
  if (value > detectionThreshold + 130)
  {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 2000); // Play 2000 Hz tone
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN); // Stop tone
  }

  // Update maximum peak (using raw value for better precision)
  if (rawValue > maxValue)
  {
    maxValue = rawValue;
    lastPeak = millis(); // Record time of new peak
  }

  // Reset max value if resetDelay elapsed without new peak
  if (millis() - lastPeak > resetDelay)
  {
    maxValue = 0;      // Reset peak
    lastPeak = millis(); // Reset timer
  }

  delay(30); // Control loop speed
}
