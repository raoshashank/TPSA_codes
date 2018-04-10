#include <Wire.h>
#define SLAVE_ADDRESS 0x06
int number = 0;
int state = 0;
char c;
int GBG,EMG,SHP,KCK,Lm,BURGLER;
//void receiveData()
//{
//    // Check if we have a 32-bit number (4 bytes) in queue
//    while(Wire.available())
//    {
//        number = 0;
//        for(int i = 0; i < 32; i += 8)
//        {
//            // This is merging the bytes into a single integer
//            number |= ((int)Wire.read() << i);
//        }
//        Serial.print("data received: ");

//        Serial.println(number);
//
//       
//    }
//}
String data_recieved_from_pi = "";

//void receiveData() {
//
//            while( Wire.available()) {
//                data_recieved_from_pi += (char)Wire.read();
//            }
//
//            Serial.print("Data Received From PI:");
//            Serial.println(data_recieved_from_pi);
//
//            data_recieved_from_pi = "";
//}
char ch = 'a';
//void sendData()
//{
////    for(int i = 0; i < 32; i += 8)
////    {
////        // This is extracting each byte from the number
////        Wire.write((number >> i) & 0xFF);
////    }
//  //Wire.write("AB");
//  String data="##04|";
//  Serial.println("Sending Data");
//  /*EMG=1;
//  SHP=2;
//  GBG=3;
//  BURGLER=4;
//  KCK=5;
//  Lm=30;*/
//  data+=EMG;
//  data+=SHP;
//  data+=GBG;
//  data+=BURGLER;
//  data+=KCK;
//  data+="000";
//  data+="|";
//  data+=Lm;
//  data+="&";
//  Serial.println("----------");
//  Serial.println(data);
//  Wire.write(data.c_str());
//}

void setup()
{
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  //Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);
  Serial.println("Ready!");
//  ch = 'A';
//  Wire.onRequest(sendData);1
}

void loop() 
 {
//  sendData();
 }
 
