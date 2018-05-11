#include <Servo.h>
Servo rmotor;
Servo lmotor;
const int rMotorPin = 10;
const int lMotorPin = 11;

//current x,y coordinates, and newX,Y coordinates
int xCalc = 160; //x, y calculated by cart after moving.
int yCalc = 120; //we can choose the most convenient starting values.
int xSeen = 160; //x, y seen by camera and sent to cart for varification.
int ySeen = 120; //xCalc will not be exactly xSeen, but knowing both helps to know the true x.
int newX = 160; //newX and newY are instructions on where to move, given by camera.
int newY = 120; //if no instruction given, cart doesn't move.
int dx = 0; //dx and dy are calculated given newX and newY.
int dy = 0; //they determine how the cart should move based on current x and y.
boolean xyCamera = false; //use the camera's x,ySeen or the cart's calculated x,yCalc to know current x,y.

//turning wheels
long angle = 9000090; //use angle % 360 to find angle from 0-360. Starts facing 90.
const long unit = 360; //this is simply a convenient constant variable.
int rstop = 93; //right motor stops given 93
int lstop = 92; //left motor stops given 92... almost...
int cxy = 20; //controls distance traveled. Constant multiplier.
int tspeed = 50; //TBD... how fast should the servos turn?
long timeLastAdjustment = 0; //angle must be adjusted, sice left motor never fully stops.

//adjusting turn amounts by hand using potentiometer
const int leftMotorTurn = A0; //pin for potentiometer1
const int rightMotorTurn = A1; //pin for potentiometer2
int leftTurnTime; //determined by potentiometer1
int rightTurnTime; //determined by potentiometer2

//set up start button
const int startButton = 12; //using pin 12 for button
boolean start = false; //do not start driving until button pressed.

void setup() {
  rmotor.attach(rMotorPin); //attatch motors to pins 10 and 11
  lmotor.attach(lMotorPin);
  pinMode(startButton, INPUT);
  Serial.begin(9600); //Serial is a writable pin on the board.
}

//**************************************************************************************** main code

void loop() {
  while(!start) { //Don't do anything until start button is pressed
    rest(10);
    if(digitalRead(startButton) == HIGH) {
      start = true;
      rest(2000); //don't move as soon as the start button is pressed, convenient delay.
    }
  }
  runCode();
  //testingCode();
}

void runCode() {
  if(Serial.available()) { //don't receive instructions which don't exist
    rx();
  }
  if(!(dx == 0 && dy == 0)) { //don't drive if it's unnecessary.
    drive();
  }
}

//***************************************************************************************** RX

void rx() {
  //Receive instruction for motion. 
  
  String coordinates = "";
  while (Serial.available()) {
    rest(10); //keep things orderly with a delay
    char c = Serial.read(); //read one character at a time
    coordinates += c; //add it to the string
  }
  
  parseCoordinatesString(coordinates); //get the information back out of the string.
  if(xyCamera) { //we can use camera's x,y to know the cart's position or the cart's calculated x,y.
    xCalc = xSeen;
    yCalc = ySeen;
  }
  dx = newX - xCalc;
  dy = newY - yCalc;
}
void parseCoordinatesString(String coordinates) {
  //get coordinates seen by camera, and desired coordinates to move to.
  xSeen = coordinates.substring(0,3).toInt() * 40;
  ySeen = coordinates.substring(3,6).toInt() * 40;
  newX = coordinates.substring(6,9).toInt() * 40;
  newY = coordinates.substring(9,12).toInt() * 40;

  //print coordinates for testing purposes.
//  Serial.print("Current X seen by camera: ");
//  Serial.println(xSeen);
//  Serial.print("Current Y seen by camera: ");
//  Serial.println(ySeen);
//  Serial.print("New X to move to: ");
//  Serial.println(newX);
//  Serial.print("New Y to move to: ");
//  Serial.println(newY);
}

//*********************************************************************************** Motion

void drive(){ 
  //move x then move y. never move diagonally.
  
  //move x first
  turnXDirection();
  straight(true); //true means we are moving in the x direction
  
  //move y next
  turnYDirection();
  straight(false); //false means we are moving in the y direction
}

void turnXDirection() {
  //facing right is 0 degrees, left is 180 degrees.
  
  if(dx > 0) { //need to move right
    if(angle%unit == 90) { //one turn right if facing up
      rturn();
    }
    else if(angle%unit == 180) { //two turns right if facing left
      rturn();
      rturn();
    }
    else if(angle%unit == 270) { //one turn left if facing down
      lturn();
    }
  }
  else if(dx < 0) { //need to move left
    if(angle%unit == 90) { //one turn left if facing up
      lturn();
    }
    else if(angle%unit == 0) { //two turns left if facing right
      lturn();
      lturn();
    }
    else if(angle%unit == 270) { //one turn right if facing down
      rturn();
    }
  }
}
void rturn() {
  rightTurnTime = (analogRead(rightMotorTurn)/4) + 500; //update turn time as needed
  rmotor.write(rstop+tspeed); //start turning motors
  lmotor.write(lstop+tspeed);
  delay(rightTurnTime); //turn 90 degrees right
  angle -= 90; //the cart always keeps track of the direction it's facing
  rest(1000);  //just relax after moving straight, keeps order.
}

void turnYDirection() {
  //facing up is 90 degrees, down is 270 degrees.
  
  if(dy > 0) { //need to move up
    if(angle%unit == 180) { //one turn right if facing left
      rturn();
    }
    else if(angle%unit == 270) { //two turns right if facing down
      rturn();
      rturn();
    }
    else if(angle%unit == 0) { //one turn left if facing left
      lturn();
    }
  }
  else if(dx < 0) { //need to move down
    if(angle%unit == 180) { //one turn left if facing left
      lturn();
    }
    else if(angle%unit == 90) { //two turns left if facing up
      lturn();
      lturn();
    }
    else if(angle%unit == 0) { //one turn right if facing right
      rturn();
    }
  }
}
void lturn() {
  leftTurnTime = (analogRead(leftMotorTurn)/4) + 500; //update turn time as needed
  rmotor.write(rstop-tspeed); //start turning motors
  lmotor.write(lstop-tspeed);
  delay(leftTurnTime); //turn 90 degrees left
  angle += 90; //the cart always keeps track of the direction it's facing
  rest(1000); //just relax after moving straight, keeps order.
}

void straight(boolean movingXDirNotYDir) {
  lmotor.write(lstop+tspeed); //start moving both motors
  rmotor.write(rstop-tspeed); //motors are oriented in opposite directions, thus (+) or (-) tspeed

  if(movingXDirNotYDir) { //we are moving in the x direction
    xCalc = newX; //calculate new x value. always use xCalc and yCalc whether using camera's xSeen and ySeen to know postion or not.
    if(dx < 0) { //always treat distance as a positive value for moving straight.
      dx = dx * (-1);
    }
    delay(dx * cxy);
    dx = 0; //we don't need to move in x anymore.
    rest(1000); //just relax after moving straight, keeps order.
  }
  
  else if(!movingXDirNotYDir){                  //moving in the y direction
    yCalc = newY; //calculate new x value. always use xCalc and yCalc whether using camera's xSeen and ySeen to know postion or not.
    if(dy < 0) { //always treat distance as a positive value for moving straight.
      dy = dy * (-1);
    }
    delay(dy * cxy);
    dy = 0; //we don't need to move in y anymore.
    rest(1000); //just relax after moving straight, keeps order.
  }

  

  return;
}

void rest(int t) {
  rmotor.write(rstop); //stop wheels
  lmotor.write(lstop);
  for(int i = 0; i < t/10; i++) { //this for loop makes resting smoother
    delay(10);
    adjustAngle(); //left motor never fully stops.
  }
}
void adjustAngle() { 
  //this method is an attempt to correct the error created by the left motor's inability to stop.
  lmotor.write(lstop + 2);
  delay((millis() - timeLastAdjustment)*3/24);
  lmotor.write(lstop);
  timeLastAdjustment = millis();
}

//************************************************************************* Testing code, can delete once finished with the project.

void testingCode() {
  //turn180(1);
  fullCircuit(1);
  //turn180(1);
//
//  Serial.print("Left: ");
//  Serial.println(analogRead(leftMotorTurn));
//  Serial.print("Right: ");
//  Serial.println(analogRead(rightMotorTurn));
  
}

void turn180(int laps) {
  for(int i = 0; i < laps; i++) {
    lturn();
    rest(10000);
    rturn();
    rest(10000);
    lturn();
    rest(10000);
    rturn();
    rest(10000);
  }
}

void fullCircuit(int laps) {
  int i = 0;
  while(i < laps) {
    dx = 80;
    dy = 60;
    drive();
    rest(1000);

    dx = -160;
    dy = 0;
    drive();
    rest(1000);

    dx = 0;
    dy = -130;
    drive();
    rest(1000);

    dx = 160;
    dy = 0;
    drive();
    rest(1000);

    dx = -80;
    dy = 60;
    drive();

    rturn();
    rturn();

    i++;   
  }
  printStats();
}

void square() {
  //starts lower left
  drive();
  dx = 80;
  drive();
  rest(250);
  dy = 80;
  drive();
  rest(250);
  dx = -80;
  drive();
  rest(250);
  dy = -80;
  drive();
  rest(250);
  dy = -80;
  drive();
  rest(250);
  dx = 80;
  drive();
  rest(250);
  rturn();
}

void printStats() {
  Serial.print("Angle: ");
  Serial.println(angle%unit);
  Serial.print("X: ");
  Serial.println(xCalc);
  Serial.print("Y: ");
  Serial.println(yCalc);
}





















