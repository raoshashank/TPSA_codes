#include <Wire.h>
#define SLAVE_ADDRESS 0x08

const int LDR_conv = A0;
const int LDR_wall = A1;

const int IR = A2;
const int LM35 = A3;

const int motor_conv_enable = 10;
const int motor_gate_enable = 11;
const int motor_conv_direction = 12;
const int motor_gate_direction = 13;

const int buzzer = 9;

const int fan = 8;

const int  push_EMG = 7;
const int  push_SHP = 6;

const int LED = 0;
String data = "@@06|";

int EMG, SHP, GBG, Lm;
String data_recieved_from_pi = "";
int gc = 0;


void receiveData() {

  while ( Wire.available())
  {
    data=Wire.read();
  }
}
void sendData()
{
  data="@@06|";
  data += String(EMG);
  data += String(SHP);
  data += String("0");
  data += String(GBG);
  data += String("000");
  data += String("|");
  data += String(Lm);
  data += String("&");
  Wire.write(data.c_str());
}

void setup()
{
  Serial.begin(250000);
  pinMode(motor_conv_enable, OUTPUT);
  pinMode(motor_conv_direction, OUTPUT);
  pinMode(motor_gate_enable, OUTPUT);
  pinMode(motor_gate_direction, OUTPUT);

  pinMode(push_SHP, INPUT);
  pinMode(push_EMG, INPUT);

  pinMode(LED, OUTPUT);
  pinMode(fan, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(LDR_conv, INPUT);
  pinMode(LDR_wall, INPUT);
  pinMode(IR, INPUT);
  pinMode(LM35, INPUT);
 
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

}


void loop()
{
  int LDR_Conv = analogRead(LDR_conv);
  int LDR_Wall = analogRead(LDR_wall);
  int Ir = analogRead(IR);
  Lm = abs((analogRead(LM35) * 500) / 1023);
  SHP = digitalRead(push_SHP);
  EMG = digitalRead(push_EMG);
 
  analogWrite(motor_conv_enable, 0);
  digitalWrite(motor_conv_direction, LOW);
  analogWrite(motor_gate_enable, 0);
  digitalWrite(motor_gate_direction, LOW);

  if (LDR_Wall < 100)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);

  if (data_recieved_from_pi[0] == 1)
  {
    EMG = 0;
  }


  if (data_recieved_from_pi[0] == 1)
  {
    GBG = 0;
  }

  if (Lm > 28)
  {
    digitalWrite(fan, HIGH);
  }
  else if (Lm > 30)
  {
    digitalWrite(fan, HIGH);
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(fan, LOW);
    digitalWrite(buzzer, LOW);
  }

  
  delay(100);

}
/*_____________________________*/

