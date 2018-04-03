#include<Servo.h>
int ldr =A0, ir_sensor=A1;
int count = 0;
int lm35 = A4;
int ir,ldr_wall;
int pos = 0;
int servo = 10;
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
  
    /*________________gate____________*/
  ir = analogRead(ir_sensor);
  Serial.println("ir "+String(ir));
  if (ir > 500)
  {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  if (ir < 500)
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
  int  temp = analogRead(lm35)*500/1023;
  Serial.println("Temp "+String(temp));
  if (temp > 28)
  {
    digitalWrite(11, HIGH);
  }
  else
  {
    digitalWrite(11, LOW);
  }
  /*________Smoke sensor____________*/
  int  smoke_sensor = analogRead(A1);
  Serial.println("Smoke "+String(smoke_sensor));
  if (smoke_sensor > 2000)
  {
    digitalWrite(13 , HIGH);
    Serial.println("Emergency");
    delay(10000);
  }
  /*___________*/
  /*______clock_________*/
  count += 10;
  if (count % 1000000 == 0)
  {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  }
  /*_________________*/
  /*_____Burglar alarm_____________*/
  ldr = 1023 - analogRead(A0);
  Serial.println("ldr "+String(ldr));
  if ( ldr < 100)
  {
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13,LOW);
  }
  /*______________*/
}
