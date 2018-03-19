/*Detects motion inside 110 degree cone in range of 3-7 metres
 * DIGITAL DEVICE
 * Settings available: 
 * Time delay adjust:Sets how long the output remains high after detecting motion
 * Anywhere from 5 seconds to 5 minutes.
 * Sensitivity adjust: Sets the detection range-from 3 meters to 7 meters
 * link for details: http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-hc-sr501-motion-sensor-tutorial/
 * pins required: 5V,GND,Data pin
 * OUTPUT OF DEVICE GOES LOW FOR APPROX 3 SECONDS AFTER TIME DELAY COMPLETES [NO MOTION DETECTION DURING THIS PERIOD]
 * The device requires nearly a minute to initialize.  
 * During this period,  it can and often will output false detection signals.  
 * Circuit or controller logic needs to take this initialization period into consideration.
 * Check if sensor is in repeatable or single mode. in repeatable mode, the time delay is reset everytime the motion is detected
 */

int ledPin = 13;  // LED on Pin 13 of Arduino
int pirPin = 7; // Input for HC-S501
int pirValue; // Place to store read PIR Value

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);
}
