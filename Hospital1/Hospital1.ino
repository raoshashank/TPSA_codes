#include <Wire.h>
#define SLAVE_ADDRESS 0x05

const int LDR_conv = A0;
const int LDR_wall = A1;

const int smoke_detector = 4; //(?)

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

int EMG,SHP,KCK,Lm;

String data_recieved_from_pi = "";
int gc=0;


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
  data+="0";
  data+=KCK;
  /*------data to send------*/
  data+="000";
  data+="|";
  data+=Lm;
  data+="&";
  Serial.println("----------");
  Serial.println(data);
  Wire.write(data.c_str());
}
/*-----------------------------------------------------------*/

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

/*-------------------------------------------------------------*/
//true = HIGH, false = LOW
void gate_actuate(bool dir)
{
  analogWrite(motor_gate_enable, 100);
  if(dir == true)    
    digitalWrite(motor_gate_direction, HIGH);
  else  digitalWrite(motor_gate_direction, LOW);  
}
/*-------------------------------------------------------------*/

void setup()
{

  pinMode(motor_conv_enable, OUTPUT);
  pinMode(motor_conv_direction, OUTPUT);
  pinMode(motor_gate_enable, OUTPUT);
  pinMode(motor_gate_direction, OUTPUT);

  pinMode(push_SHP, INPUT);
  pinMode(push_EMG, INPUT);

  pinMode(LED, OUTPUT);
  pinMode(fan, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(LDR_conv,INPUT);
  pinMode(LDR_wall, INPUT);
  pinMode(smoke_detector, INPUT);
  pinMode(IR, INPUT);
  pinMode(LM35, INPUT);
  Serial.begin(9600);

}

/*TODO
 * LDR-CONVEYOR 
 * IR and Gate
 * LM35 and fan
 * smoke and LM35 and buzzer
 * 2 buttons
 * ldr and led
 * LCD display
 */


void loop() 
{
  int LDR_Conv = analogRead(LDR_conv);
  int LDR_Wall = analogRead(LDR_wall);
  int Smoke_Detector = analogRead(smoke_detector);
  int Ir = analogRead(IR);
  Lm = (analogRead(LM35) * 500) / 1023;
  SHP=digitalRead(push_SHP);
  EMG=digitalRead(push_EMG);

  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");
 
/*___________________conv_motor____________*/
  if (LDR_conv < 250)
  {
    analogWrite(motor_conv_enable, 255);
    delay(1000);
    analogWrite(motor_conv_direction, 0);
    delay(5000);
    analogWrite(motor_conv_enable,0);
  }
  
/* ____________________________________________*/


 /*________________ldr__________________*/
  if (LDR_Wall < 100)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED,LOW);
 /*______________________________________*/


    
/*_____________________gate______________*/
  if(Ir<400 && gc==0)
  {
    analogWrite(motor_conv_enable, 255);
    digitalWrite(motor_conv_direction,HIGH);
    delay(3000);
    Serial.println("close");
    analogWrite(motor_conv_enable, 0);
    delay(3000);
    digitalWrite(motor_conv_direction,LOW);
    analogWrite(motor_conv_enable, 255);
    Serial.println("open");
    delay(3000);
  } 

  else if(Ir>400)
    gc = 0;
/*________________________________________________*/


  
/*_____________________________lm35_______________*/
    
    if (Lm>28)
    {
      digitalWrite(fan,HIGH);
    }
    else if(Lm>30)
    {
      digitalWrite(fan,HIGH);
      digitalWrite(buzzer,HIGH);
    }
    else
    {
      digitalWrite(fan,LOW);
      digitalWrite(buzzer,LOW);
    }

/*______________________________________________________*/
    //ALL  OUTPUTS
  /*--------------------------------------------------*/
  Serial.println("LDR conv = " + String(LDR_Conv));
  Serial.println("LDR Wall = " + String(LDR_Wall));
  Serial.println("Smoke = " + String(Smoke_Detector));
  Serial.println("IR = " + String(Ir));
  Serial.println("LM35 = " + String(abs(Lm)));
  Serial.println("SHP="+String(SHP));
  Serial.println("EMG="+String(EMG));
  Serial.println("---------------------------");
  /*--------------------------------------------------*/ 
  delay(10);
    
}
  /*_____________________________*/
  
