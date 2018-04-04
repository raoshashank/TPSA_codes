#define Bit_out 4 // Pin 1-2 SN74164
#define CLK 5 // Pin 9 Sn74164
#define RS A3  // Pin 4 LCD
#define E 6  // Pin 6 LCD

#define Line1 0x80  // location LCD row 0 col 0 or line 1 LCD
#define Line2 0x80 + 0x40  // location row 1 col 0 or line 2 LCD

void setup() {
  pinMode(Bit_out, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);

  digitalWrite(CLK, LOW); 
  digitalWrite(RS, LOW); // LCD in command mode default
  digitalWrite(E, HIGH);

  initLCD(); // see below

}

void loop() {
//   below simply prints "Hello World!" on line 1
    char String1[] = "Hello world!";
  typeln(String1, Line1);

  
  delay(500);
  ClearDisplay();

} // end loop

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
  for (int j=1; j<=8; j++)  {   // shift out MSB first
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

