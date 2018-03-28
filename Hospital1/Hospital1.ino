const int LDR_CONV= A0;
const int LDR_Wall =A1;
const int IR = A2;
const int Smoke_detector = A3;
const int Lm =A4;
const int PIR ;
const int motor_conveyor_en;
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
  pinMode(Lm,INPUT);
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
  analogWrite(motor_gate_en, 100);
  if(dir == true)    
    digitalWrite(motor_gate_dir, HIGH);
  else  digitalWrite(motor_gate_dir, LOW);  
}
/*-----------------------------*/

int count = 0;
int smoke_threshold;
int LDR_CONV_threshold;

void loop()
{
  int LDR_CONV = analogRead(LDR_CONV);
  int LDR_Wall = analogRead(LDR_Wall);
  int Smoke_detector = analogRead(Smoke_detector);
  int IR = analogRead(IR);
  int Lm = (analogRead(Lm) * 500) / 1024;

  digitalWrite(LED_CONV,HIGH);
  /*------CONVEYOR BASED ON LDR----*/
  if(LDR_CONV<LDR_CONV_threshold)
  {
   digitalWrite(motor_conveyor_en,100);
   digitalWrite(motor_conveyor_dir,HIGH);
   delay(5000); 
  }
  /*------------------------------*/

  
  /*---------------Gate closing and Opening when IR is blocked--------*/
  //count= becomes zero to reset IR sensing
  if (IR < 100 && count == 0)
  {
    //USE motor function here.
    gate_actuate(true);
    delay(3000);
    analogWrite(motor_gate_en, 0);
    delay(1500);
    gate_actuate(false);
    delay(3000);
    analogWrite(motor_gate_en, 0);
    count++;
  }
  
  else if (IR > 100)
  {
    analogWrite(motor_gate_en, 0);
    count = 0;
  }
  /*--------------------------------------------------*/
  
  
  /*-----------LM35-fan---------------------------------------*/
  if (Lm > 30)
  {
    analogWrite(motor_fan_en, 50);
    digitalWrite(motor_fan_dir, HIGH);
  }
  else
    analogWrite(motor_fan_en, 0);
  /*--------------------------------------------------*/
  /*------------SMOKE DETECTOR--------------------------------------*/
  //TODO: Calibrate the smoke sensor threshold value
  smoke_threshold=0;
  if (Smoke_detector > smoke_threshold)
  {
    //SEND EMERGENCY REQUEST TO RPI
    gate_actuate(true);  
  }
  /*--------------------------------------------------*/  
  
  
  /*call_ambulance ,mall,emergency here*/
 
}


