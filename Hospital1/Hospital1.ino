#include <Wire.h>
#define SLAVE_ADDRESS 0x08
const int baud_rate=9600;
const int LDR_conv = A0;
const int LDR_wall = A1;
const int SMOKE = 2;

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
String data = "@@08|";

int EMG, SHP, GBG, Lm,gate_st,smoke,conv_st;
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
  data="@@08|";
  data += String(0);
  data += String(0);
  data += String(0);
  //data += String("0");
  data+="0";//LCD ??
  data+=String(smoke);
  data+="0"; //Garbage IR
  data+=String(gate_st);
  data += String("|");
  data+=String(conv_st);
  data += String(Lm);
  data += String("&");
  Wire.write(data.c_str());
}

void setup()
{
  Serial.begin(baud_rate);
  pinMode(motor_conv_enable, OUTPUT);
  pinMode(motor_conv_direction, OUTPUT);
  pinMode(motor_gate_enable, OUTPUT);
  pinMode(motor_gate_direction, OUTPUT);

  pinMode(push_SHP, INPUT);
  pinMode(push_EMG, INPUT);
  pinMode(SMOKE,INPUT);
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
  smoke=digitalRead(SMOKE);
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

//  if (data_recieved_from_pi[0] == 1)
//  {
//    EMG = 0;
//  }
//
//
//  if (data_recieved_from_pi[0] == 1)
//  {
//    GBG = 0;
//  }

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

  if(LDR_Conv<200)
  {
    conv_st=1;
    analogWrite(motor_conv_enable, 255);
    digitalWrite(motor_conv_direction, HIGH);
  }
  else {
    conv_st=0;
    analogWrite(motor_conv_enable, 0);
  }

  if(Ir>200)
  { gate_st=1;
    analogWrite(motor_gate_enable,255);
    digitalWrite(motor_gate_direction,HIGH);
    delay(300);
    analogWrite(motor_gate_enable, 0);
    delay(600);
    digitalWrite(motor_gate_direction,LOW);
    analogWrite(motor_gate_enable,255);
    delay(300);
    analogWrite(motor_gate_enable,0);
  }
  else{
    gate_st=0;    
  }
  

  if(smoke==1)
  {
        digitalWrite(fan, HIGH);
        EMG=1;
  }
  else{
    EMG=0;
  }
  

//  Serial.println("LDR conv = " + String(conv_st));
  
  Serial.println("---------------------------");
  
  delay(1000);

}
/*_____________________________*/

