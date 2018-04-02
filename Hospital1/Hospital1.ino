int ldr_conv, ldr_wall, ir;
int Lm35,Smoke_detector; 
int count = 0;
int gc = 0;
const int temp_fan=8;
int led;
int buzz=9;
const int conv_motor_speed = 10;
const int conv_motot_dir = 12;
const int gate_speed = 13;
const int gate_dir = 11;
void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  Serial.begin(9600);



  // put your setup code here, to run once:

}

void loop() {
  ldr_conv = 1023 - analogRead(A0);
  ldr_wall = analogRead(A1);
  ir = 1023 - analogRead(A2);
  Lm35 = (analogRead(A3)*500)/1023;
  Smoke_detector = analogRead(A4);
  Serial.println("Lm35 =" + String(Lm35));
  Serial.println("ldr_conv=" + String(ldr_conv));
  Serial.println("ldr wall=" + String(ldr_wall));
  Serial.println("ir=" + String(ir));
  Serial.println("Smoke = " + String(Smoke_detector));

  delay(1000);
  /*___________________conv_moto____________*/
  if (ldr_conv < 250 && count == 0)
  {
    analogWrite(conv_motor_speed, 255);
    delay(1000);
    analogWrite(conv_motor_speed, 0);
    count++;
  }
  else if(ldr_conv>250)
  {
    count = 0;
  }
 /* ____________________________________________*/


  /*________________ldr__________________*/
  if (ldr_wall < 100)
   { digitalWrite(led, HIGH);
    delay(1000);
  }


  /*_____________________gate______________*/
  if(ir<400 && gc==0)
{analogWrite(gate_speed, 255);
    digitalWrite(gate_dir,HIGH);
    delay(3000);
    Serial.println("close");
    analogWrite(gate_speed, 0);
    delay(3000);
    digitalWrite(gate_dir,LOW);
    analogWrite(gate_speed, 255);
    Serial.println("open");
    delay(3000);


}

  else if(ir>400)
    gc = 0;
/*________________________________________________*/


  
/*_____________________________lm35_______________*/
    
    if (Lm35>28)
    {
      digitalWrite(temp_fan,HIGH);
    }
    else if(Lm35>30)
    {
      digitalWrite(temp_fan,HIGH);
      digitalWrite(buzz,HIGH);
    }
    else
    {
      digitalWrite(temp_fan,LOW);
      digitalWrite(buzz,LOW);
    }
    
}
  /*_____________________________*/
  
