#include<Servo.h>
#include<Wire.h>
#define SLAVE_ADDRESS 0x03
const int IR = A2;
int pos = 0;
int ir_enable = 500;
const int LM35 = A0;
int temp_max = 28;
const int SMOKE = A1;
int smoke_max = 200;
int LDR_wall = A4;
int burg = 300;
Servo myservo;
const int  push_EMG = 7;
const int  push_SHP = 6;

const int LED = 0;

int EMG, SHP, KCK, Lm;

String data_recieved_from_pi = "";
int gc = 0;


void setup() {
  pinMode(LM35, INPUT);
  pinMode(SMOKE, INPUT);
  pinMode(IR, INPUT);
  pinMode(LDR_wall, INPUT);
  pinMode(12, OUTPUT);
  myservo.attach(10);
  pinMode(push_SHP, INPUT);
  pinMode(push_EMG, INPUT);
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
void receiveData() {

            while( Wire.available()) {
                data_recieved_from_pi += (char)Wire.read();
            }

            Serial.print("Data Received From PI:");
            Serial.println(data_recieved_from_pi);

            data_recieved_from_pi = "";
}
/*------------------------------------*/

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
  /*________________delivery mechanism____________*/
  int ir = analogRead(IR);
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
    delay(3000);
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  /*_________________________________*/
  // HIGH = Buzzer on , LOW = Buzzer off
  //TODO : Smoke sensor integration

  /*______________buzzer_______________*/
  int  temp = analogRead(LM35) * 500 / 1023;
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
  int  smoke_sensor = analogRead(SMOKE);
  Serial.println("Smoke " + String(smoke_sensor));
  if (smoke_sensor > smoke_max)
  {
    digitalWrite(13 , HIGH);
    Serial.println("Emergency");
    delay(10000);
    EMG = 1;
  }
  /*___________*/
  /*_____Burglar alarm_____________*/
  int ldr = 1023 - analogRead(LDR_wall);
  Serial.println("ldr " + String(ldr));
  if ( ldr < burg)
  {
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);
  }
  /*______________*/
}
