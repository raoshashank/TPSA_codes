#include <Wire.h>
#define SLAVE_ADDRESS 0x06
const int baud_rate=9600;
const int LDR_top = A1;
const int LDR_wall = A0;
const int smoke_detector = 4;
const int IR = A3;
const int LM35 = A2;

const int motor_gate_enable = 11;
const int motor_fan_enable = 10;
const int motor_gate_direction = 13;
const int motor_fan_direction = 12;

const int laser = 9;

const int  push_GBG = 5;
const int  push_EMG = 7;
const int  push_SHP = 6;

const int LED_1 = 1;

const int buzzer = 8;
int GBG, EMG, SHP, KCK, Lm, BURGLER,smart_light,smoke;

String data = "@@06|";

void receiveData()
{
  while ( Wire.available()) {
    data = Wire.read();
  }

  data = "";
}

void sendData()
{
  data = "@@06|";
  data += String(EMG);
  data += String(SHP);
  data += String(GBG);
  data+="0";//Garbage IR
  data+=String(smart_light);
  data += String(BURGLER);
  data += String(KCK);
  data += String(smoke);
  data+="0";
  data += "|";
  data += String(Lm);
  data += "&";
  Wire.write(data.c_str());
}

void setup()
{ 
  Serial.begin(baud_rate);
  pinMode(motor_gate_enable, OUTPUT);
  pinMode(motor_gate_direction, OUTPUT);
  pinMode(motor_fan_enable, OUTPUT);
  pinMode(motor_fan_direction, OUTPUT);

  pinMode(push_SHP, INPUT);
  pinMode(push_EMG, INPUT);
  pinMode(push_GBG, INPUT);

  pinMode(LED_1, OUTPUT);

  pinMode(laser, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(LDR_top, INPUT);
  pinMode(LDR_wall, INPUT);
  pinMode(smoke_detector, INPUT);
  pinMode(IR, INPUT);
  pinMode(LM35, INPUT);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

int count = 0;

void loop()
{

  int LDR_Top = analogRead(LDR_top);
  int LDR_Wall = analogRead(LDR_wall);
  int Smoke_Detector = digitalRead(smoke_detector);
  int Ir = analogRead(IR);
  Lm = abs((analogRead(LM35) * 500) / 1023);
  GBG = digitalRead(push_GBG);
  SHP = digitalRead(push_SHP);
  EMG = digitalRead(push_EMG);
  smart_light=0;

  digitalWrite(laser, HIGH);

  if (data[0] == 1)
  {
    EMG = 0;
  }


  if (data[0] == 1)
  {
    SHP = 0;
  }

  if (data[0] == 1)
  {
    GBG = 0;
  }

  if (LDR_Wall < 150)
  {
    digitalWrite(buzzer, HIGH);
    BURGLER = 1;
  }
  else {
    digitalWrite(buzzer, LOW);
    BURGLER = 0;
  }

  if(Smoke_Detector==1){
    smoke=1;
    analogWrite(motor_fan_enable, 255);
    digitalWrite(motor_fan_direction, HIGH);
    EMG=1;
  }
  else{
    smoke=0;
    analogWrite(motor_fan_enable, 0);
    EMG=0;
  }

  if (Ir < 100 && count == 0)
  {

    analogWrite(motor_gate_enable, 255);
    digitalWrite(motor_gate_direction, HIGH);
    delay(300);
    analogWrite(motor_gate_enable, 0);
    delay(1000);
    digitalWrite(motor_gate_direction, LOW);
    analogWrite(motor_gate_enable, 255);
    delay(300);
    //delay(700);
    //digitalWrite(motor_gate_direction, LOW);
    //analogWrite(motor_gate_enable, 100);
    //delay(300);
    analogWrite(motor_gate_enable, 0);
    count++;
  }

  else if (Ir > 100)
  {
    analogWrite(motor_gate_enable, 0);
    count = 0;
  }

  if (Lm > 22)
  {
    analogWrite(motor_fan_enable, 255);
    digitalWrite(motor_fan_direction, HIGH);
  }
  else
    analogWrite(motor_fan_enable, 0);

  if (LDR_Wall < 300)
  {
    digitalWrite(LED_1, HIGH);
    
  }
  else {
    digitalWrite(LED_1, LOW);
    
  }
  if (LDR_Top >300)
  {
    smart_light=1;
    digitalWrite(LED_1,HIGH);
  }
  else{
    digitalWrite(LED_1,LOW);
  }

  if (EMG == 1 && data[4] == 1)
  {
    EMG = 0;
  }
  Serial.println("Ldr Wall: " +String(LDR_Wall));
  Serial.println("IR: " +String(Ir));
  Serial.println("LM: " +String(Lm));
  

}
