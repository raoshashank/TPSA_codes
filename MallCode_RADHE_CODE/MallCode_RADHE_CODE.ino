#include<Servo.h>
#include<Wire.h>
#define SLAVE_ADDRESS 0x07
const int baud_rate=9600;
#define Bit_out 4 // Pin 1-2 SN74164
#define CLK 5 // Pin 9 Sn74164
#define RS 0  // Pin 4 LCD
#define E 1  // Pin 6 LCD

#define Line1 0x80  // location LCD row 0 col 0 or line 1 LCD
#define Line2 0x80 + 0x40  // location row 1 col 0 or line 2 LCD
int bot_detect;
int angle=120;
int gbg_count=0;
const int trigPin = 6;
const int echoPin = A3;
const int grnd = 12;
const int SMOKE = A0;
const int LM35 = A1;
const int LDR = A2;
int fan_enable = 9;
int fan_input = 10;
const int servo_gbg=9;
const int ir_gbg=2;

long duration;
int distance;
Servo myservo;
Servo bin;
int servo = 11 ;
int pos = 0;

const int  push_EMG = 7;
//int push_SHP = 13;

int buzzer = 8;

const int LED = 2;
int clk = 3;
int count = 0;

int temp_max = 31;
int smoke_max = 300;
int burg = 300;
int EMG, Lm,smoke,ldr_st;
int ldr_val;

String data= "";
int gc = 0;
int ultra_distance();

void setup() {

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(grnd, OUTPUT);
  pinMode(LM35, INPUT);
  pinMode(SMOKE, INPUT);
  pinMode(fan_enable, OUTPUT);
  myservo.attach(servo);
  pinMode(push_EMG, INPUT);
  pinMode(fan_input,OUTPUT);
  pinMode(Bit_out, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RS, OUTPUT);
  pinMode(LDR,INPUT);
  pinMode(E, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(CLK, LOW);
  digitalWrite(RS, LOW); // LCD in command mode default
  digitalWrite(E, HIGH);
  bin.attach(servo_gbg); //3 is the pwm pin for servo actuation
  pinMode(ir_gbg,INPUT);
  
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onRequest(sendData);
  initLCD();
  digitalWrite(grnd, LOW);
//  Serial.begin(9600);

}

void typeInt(int k, int location)   {
  char array1[10];
  itoa(k, array1, 10); // int to string
  typeln(array1, location);
}

void typeChar(byte val)   {
  ssrWrite(val);
  digitalWrite(RS, HIGH);
  pulseOut(E);
  digitalWrite(RS, LOW);
}

void writeCommand(byte val)   {

  ssrWrite(val); // send byte to 74164
  digitalWrite(RS, LOW); // make sure RS in Com mode
  pulseOut(E);
}


// Below we pass a pointer to array1[0].
void typeln(char *s, int location)   {
  //delayMicroseconds(1000);
  writeCommand(location); // where to begin
  while (*s)  typeChar(*(s++));
}  // end typeln

// inverts state of pin, delays, then reverts state back
void pulseOut(byte x)   {
  byte z = digitalRead(x);
  //delayMicroseconds(10);
  z = !z; // reverse state
  digitalWrite(x, z);
  z = !z; // return to original state
  digitalWrite(x, z);
} // end pulsout()

/*
  To shift LSB out first:
  byte  temp = val & B00000001;
  if (temp == 0x01) digitalWrite(Bit_out, HIGH);
  else digitalWrite(Bit_out, LOW);
  pulsout(CLK);
  val = val >> 1; // shift one place right
*/

void ssrWrite(byte val)  {  // shift data to 74164
  for (int j = 1; j <= 8; j++)  { // shift out MSB first
    byte  temp = val & B10000000; // MSB out first
    if (temp == 0x80) digitalWrite(Bit_out, HIGH);
    else digitalWrite(Bit_out, LOW);
    pulseOut(CLK);
    val = val << 1; // shift one place left
  }  // next j
}  // end byteWrite


/*
  Hd44780 display commands:
  0x0f = initiate display cursor on blinking
  0x0c = initiate display cursor off
  0x01 = clear display fills display with spaces (0x20).
  0x02 = HOME returns to line one first character
  0x38 = 2 lines X 16 char 8 bits mode. Defaults to 1 line mode.
  0x10 = cursor left
  0x14 = cursor right
  0x18 = Shifts entire display left
  0x1c = Shifts entire display right
  One can also go to a specific location.
  writeCommand(0x80); // begin on 1st line
  writeCommand(0x80 + 0x40); // begin on 2nd line
  writeCommand(0x38); // setup for 2 lines
  writeCommand(0x0F); // blinking cursor
  writeCommand(0x02); // home
  writeCommand(0x01); // clear
*/


void initLCD(void)   {

  writeCommand(0x38); // setup for 2 lines
  //writeCommand(0x0F); // blinking cursor
  writeCommand(0x01); // clear
  writeCommand(0x02); // home
}


void ClearDisplay(void)   {
  writeCommand(0x01); // clear
  writeCommand(0x02); // home
}

void sendData()
{
  String data = "@@05|";
  data += String(0);
  data += "0";
  data += "0";
  data += "0";
  /*------data to send------*/
  data += String(smoke);
  data+="00";
  data+=String(
  data += String("|");
  data += String(Lm);
  data += String("&");
  Wire.write(data.c_str());
}
/*-----------------------------------------------------------*/
void receiveData() {

  while ( Wire.available()) {
        data = Wire.read();
        }
  
}
/*------------------------------------*/

void loop() {
  // initialize i2c as slave
  /*-----DISPLAY-------*/
  //  below simply prints "Hello World!" on line 1
  char String1[] = "Deliveries dispatched: ";

   //delay(3000);
  char String2[] = "WELCOME TO THE MALL";
 typeln(String2, Line2);
 typeln(String1,Line1);
int index = sizeof(String1);
int count;
bot_detect=digitalRead(ir_gbg);

for (count =0;count<index;count++)
{
  writeCommand(0x18);

}
  //push buttons
  EMG = digitalRead(push_EMG);
  /*________________delivery mechanism____________*/


    distance = ultra_distance();
   if(distance < 15)
  {
      pos = 90;
    myservo.write(pos);
   
    }
  
    else
    {pos = 0;
    myservo.write(pos);
    }

  //COMMUNICATION ARRAY
  /*_________________________________*/
  // HIGH = Buzzer on , LOW = Buzzer off
  //TODO : Smoke sensor integration

  /*______________buzzer_______________*/

  
int temp = analogRead(LM35) * 28 / 65;

//Serial.print("Temparature=");
//Serial.println(temp);
  digitalWrite(fan_enable,HIGH);
  


if (temp > temp_max)
  {
    digitalWrite(fan_input, HIGH);

  }
  else
  {
    digitalWrite(fan_input, LOW);

  }
  /*________Smoke sensor____________*/
  int  smoke_sensor = analogRead(SMOKE);
//typeInt(smoke_sensor,Line1);
//Serial.print("Smoke =");
//Serial.println(smoke_sensor);

delay(500); 
  if (smoke_sensor > smoke_max)
  {
    digitalWrite(buzzer , HIGH);
    EMG = 1;
    smoke=1;
    //delay(10000);
  }
  else
 {
    smoke=0;
    digitalWrite(buzzer , LOW);
     //delay(10000);
  }
 
 if(bot_detect==0 && gbg_count==0)
    {
      gbg_count=1;
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
    else if(bot_detect==1)
    {
      gbg_count=0;
    }


  
  
  /*___LDR CODE________*/

int  ldr_value = analogRead(LDR);
//
//if(ldr_value>100)
//  
//typeInt(ldr_value,Line2);
delay(500);
//Serial.println(ldr_value);
Serial.println("IR GBG"+String(ir_gbg));
//Serial.println("LDR"+String(ldr_value));
}

int ultra_distance()
{
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;



// Serial.print("distance=");
//Serial.println(distance);
return distance;

}

