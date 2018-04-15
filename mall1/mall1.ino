#include<Servo.h>
#include<Wire.h>
#define SLAVE_ADDRESS 0x07

#define Bit_out 4 // Pin 1-2 SN74164
#define CLK 5 // Pin 9 Sn74164
#define RS A3  // Pin 4 LCD
#define E 6  // Pin 6 LCD

#define Line1 0x80  // location LCD row 0 col 0 or line 1 LCD
#define Line2 0x80 + 0x40  // location row 1 col 0 or line 2 LCD

//const int IR = A2;
int pos = 0;
//int ir_enable = 500;
const int LM35 = A1;
int temp_max = 28;
const int SMOKE = A0;
int smoke_max = 200;
int burg = 300;
Servo myservo;
const int  push_EMG = 7;
int buzzer = 8;
int conv = 9;
int conv_input = 10;
int conv_direction = 12;

int push_SHP = 13;

const int LED = 2;
int clk = 3;
int count = 0;

int EMG, SHP, KCK, Lm,conv_st;

String data = "";
int gc = 0;


void setup() {  
  Serial.begin(250000);
  pinMode(LM35, INPUT);
  pinMode(SMOKE, INPUT);
  pinMode(IR, INPUT);
  pinMode(12, OUTPUT);
  myservo.attach(10);
  pinMode(push_SHP, INPUT);
  pinMode(push_EMG, INPUT);
  pinMode(11, OUTPUT);
  pinMode(Bit_out, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);

  digitalWrite(CLK, LOW);
  digitalWrite(RS, LOW); // LCD in command mode default
  digitalWrite(E, HIGH);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  initLCD();


}
void sendData()
{
  data="@@07|";
  data += String(EMG);
  data += String(SHP);
  data += String("0");
  data += String("00000000");
  data += String("|");
  data += String(Lm);
  data += "&";
  Wire.write(data.c_str());
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
  writeCommand(0x0F); // blinking cursor
  writeCommand(0x01); // clear
  writeCommand(0x02); // home
}


void ClearDisplay(void)   {
  writeCommand(0x01); // clear
  writeCommand(0x02); // home
}



void receiveData() {

  while ( Wire.available())
  {
    data=Wire.read();
  }
}
/*------------------------------------*/

void loop() {
  // initialize i2c as slave
 
  //push buttons
  EMG = digitalRead(push_EMG);
  /*________________delivery mechanism____________*/
  int ir = analogRead(IR);
  count++;

  //COMMUNICATION ARRAY
  /*_________________________________*/
  // HIGH = Buzzer on , LOW = Buzzer off
  //TODO : Smoke sensor integration

  /*______________buzzer_______________*/
  int  temp = analogRead(LM35) * 500 / 1023;
  if (temp > temp_max)
  {
    digitalWrite(11, HIGH);
  }
  else
  {
    digitalWrite(11, LOW);
  }
  /*________Smoke sensor____________*/
  int  smoke_sensor = analogRead(SMOKE);
  if (smoke_sensor > smoke_max)
  {
    digitalWrite(13 , HIGH);
    //delay(10000);
    EMG = 1;
  }
  /*___________*/

  /*________Number of deliveries_______*/


  /*-----DISPLAY-------*/
  //   below simply prints "Hello World!" on line 1
  char String1[] = "Deliveries dispatched: ";
  typeln(String1, Line1);
  //delay(3000);
  char String2[] = "WELCOME TO THE MALL";
  typeln(String2, Line1);

  //delay(500);
  ClearDisplay();
  /*---------------*/

  /*---Conveyer----*/
  digitalWrite(conv, HIGH);
//  if ( data[5] == 1 )
//  { 
    if (ir > ir_enable)
    {
      conv_st=1;
      digitalWrite(conv_input,HIGH);
      digitalWrite(conv_direction,HIGH);
      //delay(4000);
      digitalWrite(conv_input,LOW);
    }
    else 
    {
      conv_st=0
      digitalWrite(conv_input,LOW);
      digitalWrite(conv_direction,LOW);
     }
//  }
    
  delay(100);
}
