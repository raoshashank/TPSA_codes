#include<Servo.h>

int bot_detect;
int angle=120;
int garbage_level,garbage_full;
Servo bin;

void setup() {
  bin.attach(3); //3 is the pwm pin for servo actuation
  pinMode(ir_gbg,INPUT); //5 is the pin for bot detection 
  Serial.begin(9600);
  bin.write(0);
  garbage_full=0;
}

void loop() { 
  
    bot_detect = digitalRead(ir_gbg);
    if(bot_detect==0)
    {
      bot_detect=digitalRead(ir_gbg);
      if(bot_detect==0)
      { 
        //Serial.println("Bot detected. Actuating garbage loader.");
        for(int i=0;i<angle;i++)
        {
          bin.write(i);
          delay(15);
        }
        //delay(2000);
        for(int i=0;i<angle;i++)
        {
          bin.write(angle-i);
          delay(15);
        }
        
       }
    }
  
  delay(500);
}
