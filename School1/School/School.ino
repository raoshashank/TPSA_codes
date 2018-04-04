#include<Servo.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x02
const int LDR_wall = A0, IR = A1;
int count = 0;
int LM35 = A4;
const int SMOKE = A2;
int pos = 0;
int servo = 10;

const int  push_EMG = 7;
const int  push_SHP = 6;

int EMG, SHP, KCK, Lm;

String data_recieved_from_pi = "";
int gc = 0;


/*-----------------------------------------------------------*/
void receiveData(int byteCount)
{
  while ( Wire.available()) {
    data_recieved_from_pi += (char)Wire.read();
  }

  // Serial.print("Data Received From PI:");
  Serial.println(data_recieved_from_pi);

  data_recieved_from_pi = "";
}

Servo myservo;


void setup() {
  pinMode(LDR_wall, INPUT);
  pinMode(IR, INPUT);
  pinMode(SMOKE, INPUT);
  pinMode(LM35, INPUT);
  pinMode(12, OUTPUT);
  myservo.attach(10);
  pinMode(11, OUTPUT);
  Serial.begin(9600);

}

void sendData()
{
  String data = "@@04|";
  data += EMG;
  data += SHP;
  data += "0";
  data += KCK;
  /*------data to send------*/
  data += "000";
  data += "|";
  data += Lm;
  data += "&";
  Serial.println("----------");
  Serial.println(data);
  Wire.write(data.c_str());
}
/*-----------------------------------------------------------*/



void loop() {

  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");

  //push buttons
  SHP = digitalRead(push_SHP);
  EMG = digitalRead(push_EMG);

  /*________________gate____________*/
  int ir = analogRead(IR);
  Serial.println("ir " + String(ir));
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
  int Lm = analogRead(LM35) * 500 / 1023;
  Serial.println("Temp " + String(Lm));
  if (Lm > 28)
  {
    digitalWrite(11, HIGH);
  }
  else
  {
    digitalWrite(11, LOW);
  }
  /*________Smoke sensor____________*/
  int  smoke_sensor = analogRead(A1);
  Serial.println("Smoke " + String(smoke_sensor));
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
  int ldr = 1023 - analogRead(A0);
  Serial.println("ldr " + String(ldr));
  if ( ldr < 100)
  {
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);
  }
  /*______________*/
}
