// Include the required Wire library for I2C<br>
#include<Wire.h>
int x = 0;
void setup() {
  // Start the I2C Bus as Master
  Wire.begin(); 
  Serial.begin(9600);
}
void loop() {
  Wire.beginTransmission(7); // transmit to device #9
  Serial.println(x);
  Wire.send(3);    
  // sends x 
  Wire.endTransmission();    // stop transmitting
  x++; // Increment x
  if (x > 5) x = 0; // `reset x once it gets 6
  delay(500);
}
