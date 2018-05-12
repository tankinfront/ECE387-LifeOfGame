#include <Wire.h>
String coor = "";
int x =0;
void setup() {
  // Define the LED pin as Output
 Serial.begin(9600);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}
void receiveEvent(int hs) {
  
  x = Wire.read();  
  coor=String (x);
 String st="99999900";
 st=st+coor.substring(0,1);
  st=st+"00";
  st=st+coor.substring(1,2);


  
  Serial.println(st);
  
  }

void loop() {
  
 
}






































