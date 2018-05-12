#include <Wire.h>;
#include <SdFat.h>
#include <SPI.h>
#include <UTFT.h>
#include <URTouch.h>
#include <UTFT_Buttons.h>
#include <UTFT_SdRaw.h>
UTFT myGLCD(CTE32_R2, 38, 39, 40, 41);

URTouch  myTouch( 6, 5, 4, 3, 2);
#define SD_CHIP_SELECT  53  // SD chip select pin
// file system object
SdFat sd;
UTFT_SdRaw myFiles(&myGLCD);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);
extern uint8_t BigFont[];

int color = 0;
word colorlist[] = {VGA_WHITE, VGA_BLACK, VGA_RED, VGA_BLUE, VGA_GREEN, VGA_FUCHSIA, VGA_YELLOW, VGA_AQUA};
int  bsize = 4;



void setup()
{
 Wire.begin(); 


Serial.println(F("Initialising SD card..."));
  bool mysd = 0;
  // see if the card is present and can be initialized:
  while (!mysd)
  {
    if (!sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED)) {
      Serial.println(F("Card failed, or not present"));
      Serial.println(F("Retrying...."));
    }
    else
    {
      mysd = 1;
      Serial.println(F("Card initialised."));
    }
  }

  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_HI);

   myFiles.load(0, 0, 240, 240, "wait4gps.RAW", 1 , 1);
  delay(1500);
  
  myGLCD.setColor(VGA_WHITE);
 Serial.begin(9600);
 

}

void loop()
{
  long x, y;
  
  while (myTouch.dataAvailable() == true)
  {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    int x1=x/40;
    Serial.print(x1);
    int y1= y/40;
    Serial.print("  ");
    Serial.print(y1);
    Serial.print("             ");


      Wire.beginTransmission(9); // transmit to device #9
     int tran=x1*10;
     tran = tran+y1;
  Wire.write(tran); 
  Serial.println(tran);// sends x 
  Wire.endTransmission();
    
    delay(500);
    if ((x!=-1) and (y!=-1))
    {
               myGLCD.fillCircle(x, y, bsize);
                delay(500);
                myGLCD.setColor(VGA_BLACK);
                 myGLCD.fillCircle(x, y, bsize);
                 myGLCD.setColor(VGA_WHITE);
                 myFiles.load(0, 0, 240, 240, "wait4gps.RAW", 1 , 1);
                delay(1500);
      }
      
    }
  }


