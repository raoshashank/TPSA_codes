#include<Servo.h>
int ir_sensor = 12;
int pos = 0;
int ir_enable = 500;
int lm35 = 11;
int temp_max = 28;
int smk = A1;
int smoke_max = 200;
int burg = 100;
Servo myservo;
void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A4, INPUT);
  pinMode(12, OUTPUT);
  myservo.attach(10);
  pinMode(11, OUTPUT);
  Serial.begin(9600);

}

void loop() {

  /*________________delivery mechanism____________*/
  int ir = analogRead(ir_sensor);
  Serial.println("ir " + String(ir));
  if (ir > ir_enable)
  {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  if (ir < ir_enable)
  {
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  /*_________________________________*/
  // HIGH = Buzzer on , LOW = Buzzer off
  //TODO : Smoke sensor integration

  /*______________buzzer_______________*/
  int  temp = analogRead(lm35) * 500 / 1023;
  Serial.println("Temp " + String(temp));
  if (temp > temp_max)
  {
    digitalWrite(11, HIGH);
  }
  else
  {
    digitalWrite(11, LOW);
  }
  /*________Smoke sensor____________*/
  int  smoke_sensor = analogRead(smk);
  Serial.println("Smoke " + String(smoke_sensor));
  if (smoke_sensor > smoke_max)
  {
    digitalWrite(13 , HIGH);
    Serial.println("Emergency");
    delay(10000);
  }
  /*___________*/
  /*_____Burglar alarm_____________*/
  int ldr = 1023 - analogRead(burg);
  Serial.println("ldr " + String(ldr));
  if ( ldr < burg)
  {
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);
  }
  /*______________*/
}
