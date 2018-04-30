#include<Servo.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x05
const int LDR_wall = A0, IR = A1;
const int baud_rate=9600;
int count = 0;
int LM35 = A2;
const int SMOKE = A3;
int pos = 0;
int servo = 10;
int buzz = 13;
int temp_fan = 12;
int fan_dir = 11;
int led = 7;

#define Bit_out 8 // Pin 1-2 SN74164
#define CLK 7 // Pin 9 Sn74164
#define RS 9  // Pin 4 LCD
#define E 4 // Pin 6 LCD

#define Line1 0x80  // location LCD row 0 col 0 or line 1 LCD
#define Line2 0x80 + 0x40  // location row 1 col 0 or line 2 LCD

const int  push_EMG = 5;
const int  push_SHP = 6;

int EMG, SHP, KCK, Lm,smoke,ir_st,ir_room_1,ir_room_2,smart_light;

String data = "@@05|";
int gc = 0;

Servo myservo;


void setup() {
  Serial.begin(baud_rate);
  pinMode(LDR_wall, INPUT);
  pinMode(IR, INPUT);
  pinMode(SMOKE, INPUT);
  pinMode(LM35, INPUT);
  pinMode(buzz, OUTPUT);
  myservo.attach(10);
  pinMode(fan_dir, OUTPUT);
  pinMode(temp_fan, OUTPUT);
  pinMode(Bit_out, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(push_EMG,INPUT);
  pinMode(push_SHP,INPUT);

  digitalWrite(CLK, LOW);
  digitalWrite(RS, LOW); // LCD in command mode default
  digitalWrite(E, HIGH);
  
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData);
  //Wire.onReceive(receiveData);
  
  initLCD(); // see below

}
void receiveData()
{
  while ( Wire.available()) {
    data=Wire.read();
  }
}

void sendData()
{
  String data = "@@05|";
  data += String(EMG);
  data += String(0);
  data+="0";
  data+=String(smoke);//smoke
  data+="0"; //garbage ir
  data+=String(!ir_st); //ir room 1
  data+="0"; //ir room 2'
  data+=String(smart_light);//ldr
  data += "|";
  data += String(abs(Lm));
  data += "&";
  Wire.write(data.c_str());
}
/*-----------------------------------------------------------*/


// location is place on LCD display.
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
  delayMicroseconds(1000);
  writeCommand(location); // where to begin
  while (*s)  typeChar(*(s++));
}  // end typeln

// inverts state of pin, delays, then reverts state back
void pulseOut(byte x)   {
  byte z = digitalRead(x);
  delayMicroseconds(10);
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


void loop() {

  //   below simply prints "Hello World!" on line 1
  char String1[] = "Hello world!";
  typeln(String1, Line1);
  //delay(500);
  //push buttons
  SHP = digitalRead(push_SHP);
  EMG = digitalRead(push_EMG);

  /*________________gate____________*/
  int ir = analogRead(IR);
  if (ir > 100)
  {
      ir_st=1;
      myservo.write(90);              // tell servo to go to position in variable 'pos'
      //delay(15);                       // waits 15ms for the servo to reach the position
    
  }
  
  else if (ir < 100)
  {
    ir_st=0;
    //for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
      myservo.write(0);              // tell servo to go to position in variable 'pos'
                     // waits 15ms for the servo to reach the position
    }
  
  
  /*_________________________________*/
  // HIGH = Buzzer on , LOW = Buzzer off
  //TODO : Smoke sensor integration

  /*______________buzzer_______________*/
  Lm = analogRead(LM35) * 500 / 1023;
  
  digitalWrite(fan_dir, HIGH);
  if (Lm > 28)
  {
    digitalWrite(temp_fan, HIGH);
  }
  else
  {
    digitalWrite(temp_fan, LOW);
  }
  /*________Smoke sensor____________*/
  int  smoke_sensor = analogRead(A1);
  if (smoke_sensor > 1000)
  {
    smoke=1;
    digitalWrite(buzz , HIGH);
    //delay(10000);
    EMG = 1;
  }
  else{
    smoke=0;
  }
  /*___________*/
  /*______clock_________*/
  count += 10;
  if (count % 1000000 == 0)
  {
    digitalWrite(buzz, HIGH);
    //delay(1000);
    digitalWrite(buzz, LOW);
  }
  
  /*_________________*/
  
  /*_____Burglar alarm_____________*/
  int ldr = 1023 - analogRead(A0);
  if ( ldr < 100)
  { 
    smart_light=1;
    digitalWrite(led, HIGH);
    //delay(5000);
    digitalWrite(led, LOW);
  }
  else{
    smart_light=0;
  }
 
  
  
}
