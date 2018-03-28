/*
 * TYPE: ANALOG DEVICE[OURS GIVES BOTH ANALOG AND DIGITAL]
 * 5V volatage input
 * 0-4.2 analog output voltage
 * direct connections no resistor required
 */
int sensorValue;
int digitalValue;
void setup()
{
  Serial.begin(9600);      // sets the serial port to 9600
  pinMode(A2, INPUT);
}
void loop()
{
  sensorValue = analogRead(0);       // read analog input pin 0
  digitalValue = digitalRead(0); 
  Serial.println(sensorValue, DEC);  // prints the value read
  Serial.println(digitalValue, DEC);
  delay(2000);                        // wait 100ms for next reading
}
