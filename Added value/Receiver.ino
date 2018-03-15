#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
int data = 0;


void setup() {
Serial.begin(9600);



lcd.begin(16,2);
lcd.clear();
}

void loop() {
  

  //data=Serial.read();
  //lcd.setCursor(0,0);
  //lcd.print(Serial.read());
 // lcd.setCursor(0,1);
  

 
}
