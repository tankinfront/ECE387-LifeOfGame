String coor;
void setup() {

  Serial.begin(9600);

}

void loop() {

  while (Serial.available()) {
    delay(2);  
    char c = Serial.read();
    coor += c;
  }

  if (coor.length() >0) {
    Serial.println(coor);

    coor="";
  } 
  //Transform string coor into 4 int values which are x1,x2,y1,y2
  //String coor="123456789000";
//String a=coor.substring(0,3);
// String b=coor.substring(3,6);
//String c=coor.substring(6,9);
//String d=coor.substring(9,12);
//int x1=a.toInt();
//int x2=b.toInt();
//int y1=c.toInt();
//int y2=d.toInt();
  

  }

