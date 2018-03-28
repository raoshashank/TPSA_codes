const int LDR_CONV= A0;
const int LDR_Wall =A1;
const int IR = A2;
const int Smoke_detector = A3;
const int LM35 =A4;
const int PIR ;
const int motor_converyor_en;
const int motor_conveyor_dir;

const int motor_gate_en;
const int motor_gate_dir;

const int motor_fan_en;
const int motor_fan_dir;

const int LED;
const int LED_CONV;
const int call_ambulance;
//const int call_mall;
const int call_emergency;

void setup()
{
  pinMode(LDR_CONV,INPUT);
  pinMode(LDR_Wall,INPUT);
  pinMode(IR,INPUT);
  pinMode(Smoke_detector,INPUT);
  pinMode(PIR,INPUT);
 
  pinMode(motor_conveyor_en,OUTPUT);
  pinMode(motor_conveyor_dir,OUTPUT);
  
  pinMode(motor_gate_en,OUTPUT);
  pinMode(motor_gate_dir,OUTPUT);
  
  pinMode(motor_fan_en,OUTPUT);
  pinMode(motor_fan_dir,OUTPUT);
  
  pinMode(LED,OUTPUT);
  
  pinMode(call_ambulance,INPUT);
  pinMode(call_emergency,INPUT);
  //pinMode(call_mall,INPUT);

 Serial.begin(9600);
}

/*+----------SUMMON CODE--------*/
void summon_ambulance()
{
  
}
void summon_mall()
{
  
}
void summon_emergency()
{
  
}
//true = HIGH, false = LOW
void gate_actuate(boolean dir)
{
  analogWrite(motor_gate_enable, 100);
  if(dir == true)    
    digitalWrite(motor_gate_direction, HIGH);
  else  digitalWrite(motor_gate_direction, LOW);  
}
/*-----------------------------*/
int count = 0;

void loop()
{
  int LDR_CONV = analogRead(LDR_CONV);
  int LDR_Wall = analogRead(LDR_Wall);
  int Smoke_detector = analogRead(Smoke_detector);
  int IR = analogRead(IR);
  int LM35 = (analogRead(LM35) * 500) / 1024;

  digitalWrite(LED_CONV,HIGH);
/*---------CONVEYOR BASED ON LDR, GATE BASED ON IR, EXHAUST BASED ON LM35,buzzer and gate based on smoke sensor,*/

/*---------------Gate closing and Opening when IR is blocked--------*/
//count= becomes zero to reset IR sensing
 if (Ir < 100 && count == 0)
 {
    //USE motor function here.
    gate_actuate(true);
    delay(3000);
    analogWrite(motor_gate_enable, 0);
    delay(1500);
    gate_actuate(false);
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
  /*------------SMOKE DETECTOR--------------------------------------*/
  //TODO: Calibrate the smoke sensor threshold value
  smoke_threshold=0;
  if (Smoke_detector > smoke_threshold)
  {
    //SEND EMERGENCY REQUEST TO RPI
    ring_alarm(1000,true);
    open_gate();    
  }
  /*--------------------------------------------------*/  
  
 
}


