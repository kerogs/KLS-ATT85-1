/*
 * ============================================================================
 * Project      : Electromagnetic Field Detector
 * Author       : Kerogs
 * Version      : 1.2-attiny
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


 #define signalPin A0 // Analog input connected to the copper loop antenna

 int buzzerPin = 8;         // Pin connected to the buzzer
 int buzzerFrequency = 5500; // Buzzer frequency in Hz (adjustable)
 // int buzzerFrequency = 2000; // Alternative frequency if needed
 int ledPin = 9;            // Pin connected to the LED
 int detectionThreshold = 200; // Baseline threshold for signal detection (generally between 200 and 350)
 
 void setup() {
   Serial.begin(9600);
   delay(1000);            // Short stabilization delay
 
   pinMode(buzzerPin, OUTPUT);
   pinMode(ledPin, OUTPUT);
 
   testAll(); // Play a startup test sequence
 }
 
 void loop() {
   int signalValue = analogRead(signalPin); // Read the signal from the antenna
 
   // --- DEBUG OUTPUT ---
   // You can delete debugs if necessary.
   Serial.print("signal: ");
   Serial.println(signalValue);
 
   Serial.print("threshold: ");
   Serial.println(detectionThreshold);
 
   Serial.print("activation: ");
   Serial.println(abs(signalValue - detectionThreshold));
 
   // --- DETECTION LOGIC ---
   if (abs(signalValue - detectionThreshold) > 130) {
     digitalWrite(ledPin, HIGH);            // Turn on the LED
     tone(buzzerPin, buzzerFrequency);       // Activate the buzzer
   } else {
     digitalWrite(ledPin, LOW);              // Turn off the LED
     noTone(buzzerPin);                      // Deactivate the buzzer
   }
 }
 
 /**
  * @brief Plays a startup test sequence on the LED and buzzer
  *        to confirm that the system is operational.
  */
 void testAll() {
   digitalWrite(ledPin, HIGH);
   tone(buzzerPin, buzzerFrequency);
   delay(500); // Sound and light for half a second
   noTone(buzzerPin);
   digitalWrite(ledPin, LOW);
   delay(1000); // Pause before normal operation
 }
 