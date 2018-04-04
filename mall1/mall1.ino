#include<Servo.h>
#include<Wire.h>
#define SLAVE_ADDRESS 0x03

#define Bit_out 4 // Pin 1-2 SN74164
#define CLK 5 // Pin 9 Sn74164
#define RS A3  // Pin 4 LCD
#define E 6  // Pin 6 LCD

#define Line1 0x80  // location LCD row 0 col 0 or line 1 LCD
#define Line2 0x80 + 0x40  // location row 1 col 0 or line 2 LCD

const int IR = A2;
int pos = 0;
int ir_enable = 500;
const int LM35 = A0;
int temp_max = 28;
const int SMOKE = A1;
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

int EMG, SHP, KCK, Lm;

String data_recieved_from_pi = "";
int gc = 0;


void setup() {
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

  initLCD();

  Serial.begin(9600);

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
  delayMicroseconds(1000);
  writeCommand(location); // where to begin
  while (*s)  typeChar(*(s++));
}  // end typeln

// inverts state of pin, delays, then reverts state back
void    pulseOut(byte x)   {
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

void sendData()
{
  String data = "@@04|";
  data += EMG;
  data += SHP;
  data += "0";
  data += KCK;
  /*------data to send------*/
  data += "000";
  data += "|";
  data += Lm;
  data += "&";
  Serial.println("----------");
  Serial.println(data);
  Wire.write(data.c_str());
}
/*-----------------------------------------------------------*/
void receiveData() {

  while ( Wire.available()) {
    data_recieved_from_pi += (char)Wire.read();
  }

  Serial.print("Data Received From PI:");
  Serial.println(data_recieved_from_pi);

  data_recieved_from_pi = "";
}
/*------------------------------------*/

void loop() {
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");

  //push buttons
  EMG = digitalRead(push_EMG);
  /*________________delivery mechanism____________*/
  int ir = analogRead(IR);
  Serial.println("ir " + String(ir));
  count++;

  //COMMUNICATION ARRAY
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");

  /*_________________________________*/
  // HIGH = Buzzer on , LOW = Buzzer off
  //TODO : Smoke sensor integration

  /*______________buzzer_______________*/
  int  temp = analogRead(LM35) * 500 / 1023;
  Serial.println("Temp " + String(temp));
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
  Serial.println("Smoke " + String(smoke_sensor));
  if (smoke_sensor > smoke_max)
  {
    digitalWrite(13 , HIGH);
    Serial.println("Emergency");
    delay(10000);
    EMG = 1;
  }
  /*___________*/

  /*________Number of deliveries_______*/


  /*-----DISPLAY-------*/
  //   below simply prints "Hello World!" on line 1
  char String1[] = "Deliveries dispatched: ";
  typeln(String1, Line1);
  delay(3000);
  char String2[] = "WELCOME TO THE MALL";
  typeln(String2, Line1);

  delay(500);
  ClearDisplay();
  /*---------------*/

  /*---Conveyer----*/
  digitalWrite(conv, HIGH);
  if ( data_recieved_from_pi[5] == 1 )
  {
    if (ir > ir_enable)
    {
      digitalWrite(conv_input,HIGH);
      digitalWrite(conv_direction,HIGH);
      delay(4000);
      digitalWrite(conv_input,LOW);
    }
    else 
    {
      digitalWrite(conv_input,LOW);
      digitalWrite(conv_direction,LOW);
     }
  }
}
