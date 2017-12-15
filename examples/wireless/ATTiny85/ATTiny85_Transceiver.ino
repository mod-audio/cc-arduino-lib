/*
  Control Chain - Wireless strech sensor

  This is a simple example of how to use a wireless sensor with Control Chain. 
  This build consists of 2 modules, one ATTiny 85 bassed module and an Arduino bassed module
  This code is meanth for the ATTiny 85 side of this build, 

  The ATTiny 85 transmits its data to the Arduino over bleutooth . 
  
  the circuit:
    * connecting the serial port of the BLE module to the the SoftwareSerial port on pin 0 and 1. 
    * power the BLE module with 5V. 
    * connect a sensor to A1, A2, or A3. In our case a stretch sensor on A3. 

  After upload this example to your Arduino connect it to the MOD using
  the Control Chain shield.
    
  For more information about Control Chain, please check:
  http://wiki.moddevices.com/wiki/Control_Chain
*/

//setting up the serial port
#include <SoftwareSerial.h>

const int RxD=2;
const int TxD=1;

int val;

SoftwareSerial BTSerial(RxD,TxD);

void setup() {
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
BTSerial.begin (9600);
}

void loop() {
//reading the sensor and converting it to a 1 byte value.
val = map(analogRead(A3), 0, 1024, 0, 250);
//sending the data
BTSerial.write(val);
delay(10);
}
