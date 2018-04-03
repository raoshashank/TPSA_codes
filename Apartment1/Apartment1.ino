#include <Wire.h>
#define SLAVE_ADDRESS 0x04
const int LDR_top = A0;
const int LDR_wall = A1;
const int smoke_detector = 4;
const int IR = A3;
const int LM35 = A2;
//const int sensor_A5 = A5;

const int motor_gate_enable = 10;
const int motor_fan_enable = 11;
const int motor_gate_direction = 12;
const int motor_fan_direction = 13;

const int ultrasonic_echo = 4;
const int ultrasonic_trigger = 2;

const int laser = 9;

const int  push_GBG = 5;
const int  push_EMG = 7;
const int  push_SHP = 6;

const int LED_1 = 0;
const int LED_2 = 1;
const int LED_3 = 3;

const int buzzer = 8;
int GBG,EMG,SHP,KCK,Lm,BURGLER;

String data_recieved_from_pi = "";







/*-----------------------------------------------------------*/
void receiveData(int byteCount) 
{
            while( Wire.available()) {
                data_recieved_from_pi += (char)Wire.read();
            }

           // Serial.print("Data Received From PI:");
            Serial.println(data_recieved_from_pi);

            data_recieved_from_pi = "";
}

void sendData()
{
  String data="@@04|";
  data+=EMG;
  data+=SHP;
  data+=GBG;
  data+=BURGLER;
  data+=KCK;
  data+="000";
  data+="|";
  data+=Lm;
  data+="&";
  Serial.println("----------");
  Serial.println(data);
  Wire.write(data.c_str());
}
/*-----------------------------------------------------------*/





//TODO: Garbage disposal:Servo mechanism to
//TODO: Use these functions instead of placing code in each if case
void ring_alarm(int time_to_ring=0,boolean stop_after=false)
{
  digitalWrite(buzzer,HIGH);
  if(stop_after)
  {
    delay(time_to_ring); 
    stop_alarm();
   } 
}

void stop_alarm()
{
  digitalWrite(buzzer,LOW);  
}

//true = HIGH, false = LOW
void gate_actuate(bool dir)
{
  analogWrite(motor_gate_enable, 100);
  if(dir == true)    
    digitalWrite(motor_gate_direction, HIGH);
  else  digitalWrite(motor_gate_direction, LOW);  
}

//TODO

void setup()
{

  pinMode(motor_gate_enable, OUTPUT);
  pinMode(motor_gate_direction, OUTPUT);
  pinMode(motor_fan_enable, OUTPUT);
  pinMode(motor_fan_direction, OUTPUT);

  pinMode(ultrasonic_echo, INPUT);
  pinMode(ultrasonic_trigger, OUTPUT);

  pinMode(push_SHP, INPUT);
  pinMode(push_EMG, INPUT);
  pinMode(push_GBG, INPUT);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(laser, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(LDR_top, INPUT);
  pinMode(LDR_wall, INPUT);
  pinMode(smoke_detector, INPUT);
  pinMode(IR, INPUT);
  pinMode(LM35, INPUT);


  Serial.begin(9600);

}

int count = 0;

void loop()
{

  int LDR_Top = analogRead(LDR_top);
  int LDR_Wall = analogRead(LDR_wall);
  int Smoke_Detector = analogRead(smoke_detector);
  int Ir = analogRead(IR);
  Lm = (analogRead(LM35) * 500) / 1023;
  GBG = digitalRead(push_GBG);
  SHP=digitalRead(push_SHP);
  EMG=digitalRead(push_EMG);

  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");
  digitalWrite(laser, HIGH);


  /*------------------Burglar Alarm--------------------------------*/
  if (LDR_Wall < 500)
  { 
    digitalWrite(buzzer, HIGH);
    BURGLER=1;
  }
  else{
    digitalWrite(buzzer, LOW);
    BURGLER=0;
  } 

    
  /*--------------------------------------------------*/
  
    
  /*---------------Gate closing and Opening when IR is blocked--------*/
  //count= becomes zero to reset IR sensing
  if (Ir < 100 && count == 0)
  {
    Serial.println("gate working");
    //USE motor function here.
    KCK=true;
    //gate_actuate(true);
    analogWrite(motor_gate_enable, 100);
    digitalWrite(motor_gate_direction, HIGH);
    delay(3000);
    analogWrite(motor_gate_enable, 0);
    delay(1500);
    digitalWrite(motor_gate_direction, LOW);
    delay(3000);
    analogWrite(motor_gate_enable, 0);
    count++;
  }
  
  else if (Ir > 100)
  {
    analogWrite(motor_gate_enable, 0);
    count = 0;
  }
  /*--------------------------------------------------*/
  /*-----------LM35---------------------------------------*/
  if (Lm > 30)
  {
    analogWrite(motor_fan_enable, 50);
    digitalWrite(motor_fan_direction, HIGH);
  }
  else
    analogWrite(motor_fan_enable, 0);
  /*--------------------------------------------------*/

  /*-----------------LDR for smart lighting---------------------------------*/
  if (LDR_wall<300)
   {
    digitalWrite(LED_1,HIGH);
    digitalWrite(LED_2,HIGH);
    digitalWrite(LED_3,HIGH);
   }
  else{
    digitalWrite(LED_1,LOW);
    digitalWrite(LED_2,LOW);
    digitalWrite(LED_3,LOW);
  }
  /*--------------------------------------------------*/

  /*------------SMOKE DETECTOR--------------------------------------*/
  //TODO: Calibrate the smoke sensor threshold value
  int smoke_threshold=0;
  if (Smoke_Detector > smoke_threshold)
  {
    //SEND EMERGENCY REQUEST TO RPI
    ring_alarm(1000,true);
    gate_actuate(true);    
  }
  /*--------------------------------------------------*/  
  

  //sendData();    
  //ALL  OUTPUTS
  /*--------------------------------------------------*/
  Serial.println("LDR top = " + String(LDR_top));
  Serial.println("LDR Wall = " + String(LDR_wall));
  Serial.println("Smoke = " + String(Smoke_Detector));
  Serial.println("IR = " + String(Ir));
  Serial.println("LM35 = " + String(abs(Lm)));
  Serial.println("GBG ="+String(GBG));
  Serial.println("SHP="+String(SHP));
  Serial.println("EMG="+String(EMG));
  Serial.println("---------------------------");
  /*--------------------------------------------------*/ 
  delay(10);




}
