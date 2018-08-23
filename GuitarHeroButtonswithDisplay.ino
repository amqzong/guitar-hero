// Guitar Hero by Love Hertz
// Kathy Lau, Sungbin Kim, Michael Wahrman, Janet Zhang, Amanda Zong

// include libraries
// setup for circuit

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

#define RA8875_INT 7
#define RA8875_CS 9
#define RA8875_RESET 8

// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

// hardcode expected button and time to be pressed arrays

int expButtonArr[20] = {1, 1, 2, 3, 1, 1, 2, 1, 1, 3, 1, 1, 2, 3, 1, 1, 2, 1, 1, 3};
int expTimeArr[20] = {1500, 2000, 2500, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000}; // in ms;

// initalize buttons with port numbers

const int buttonPinRed = A4;
const int buttonPinBlue = 6;
const int buttonPinWhite = A3;

// Variables will change:
int buttonStateRed = 0;         // current state of the button
int buttonStateWhite = 0;
int buttonStateBlue = 0;

int lastButtonStateRed;
int lastButtonStateWhite;
int lastButtonStateBlue;

int buttonPressed;
long timePressed;

int expButtonPressed;
long expTimePressed = 0;
int accuracy = 0;

int loopNum = 0;
int timeNow;

int positionRed;
int positionWhite;
int positionBlue;

int startTime = 4000;

void setup() {
  // initialize the button pin as a input:
  Serial.begin(9600);
  
  pinMode(buttonPinRed, INPUT);
  pinMode(buttonPinWhite, INPUT);
  pinMode(buttonPinBlue, INPUT);
  // initialize serial communication:

  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) 
  {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

    // With hardware accelleration this is instant
  tft.fillScreen(RA8875_BLACK);

  // Play with PWM
  for (uint8_t i = 255; i != 0; i -= 5 )
  {
    tft.PWM1out(i);
    delay(10);
  }
  for (uint8_t i = 0; i != 255; i += 5 )
  {
    tft.PWM1out(i);
    delay(10);
  }
  tft.PWM1out(255);


  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
}


void loop() {
  
  timeNow = millis();
  tft.fillRect(720, 0, 10, 400, RA8875_GREEN);
  
  /* Switch to text mode */  
  
  tft.textMode();
  tft.textSetCursor(600, 400);
  tft.textEnlarge(1);

  //Serial.println("Expected Time: " + String(expTimePressed) + " " + String(expButtonPressed));

  
  /* Render some text! */
  
  tft.textTransparent(RA8875_BLACK);
  tft.textWrite("Score: ");

  char buffer1[10];
  int accuracyCopy = accuracy;

  itoa(accuracyCopy, buffer1, 10);
  
  tft.textWrite(buffer1);
  
  
  //tft.fillRect(0, 0, 480, 480, RA8875_BLACK);
  
  if (timeNow % 1 == 0)
  {
    //Serial.println(timeNow);
    //Serial.println("Number of buttons pressed is " + String(buttonsHigh));
  
    //generateExpectedNote();
    //expButtonPressed = random(1,4);
  
    expButtonPressed = expButtonArr[loopNum];

    //Serial.println(expButtonPressed);
    
    expTimePressed = expTimeArr[loopNum];

    // display red note

    loopNum++;
  
    if (expButtonPressed == 1)
    {positionRed = 0;}
    
    while (positionRed >= 0 && positionRed < 680)
    {
      tft.fillRect(positionRed, 0, 50, 50, RA8875_BLACK);
      positionRed = positionRed + 1;
      tft.fillRect(positionRed, 0, 50, 50, RA8875_RED);
    }

    if (positionRed == 680)
    {tft.fillRect(0, 0, 700, 700, RA8875_BLACK);}

    // display white note
  
    if (expButtonPressed == 2)
    {positionWhite = 0;}
    
    while (positionWhite >=0 && positionWhite < 680)
    {
      tft.fillRect(positionWhite, 100, 50, 50, RA8875_BLACK);
      positionWhite = positionWhite + 1;
      tft.fillRect(positionWhite, 100, 50, 50, RA8875_WHITE);
    }

    if (positionWhite == 680)
    {tft.fillRect(0, 100, 700, 700, RA8875_BLACK);}

    // display blue note

    if (expButtonPressed == 3)
    {positionBlue = 0;}
    
    while (positionBlue >= 0 && positionBlue < 680)
    {
      tft.fillRect(positionBlue, 200, 50, 50, RA8875_BLACK);
      positionBlue = positionBlue + 1;
      tft.fillRect(positionBlue, 200, 50, 50, RA8875_BLUE);
    }

    if (positionBlue == 680)
    {tft.fillRect(0, 200, 700, 700, RA8875_BLACK);}
    
    tft.fillRect(600,400,200,100,RA8875_YELLOW);
  

  }    

    
    
  
  buttonStateRed = digitalRead(buttonPinRed);
  buttonStateWhite = digitalRead(buttonPinWhite);
  buttonStateBlue = digitalRead(buttonPinBlue);

  int buttonsHigh = 3 - (buttonStateRed + buttonStateWhite + buttonStateBlue);
  
  if (buttonsHigh == 1)
  {
    if (expButtonPressed == 1) 
    {timePressed = doButtonRed();}
    else if (expButtonPressed == 2) 
    {timePressed = doButtonWhite();}
    else if (expButtonPressed == 3) 
    {timePressed = doButtonBlue();}
    if (timePressed != -1 ) 
    {checkAccuracy(expTimePressed,timePressed,expButtonPressed,buttonPressed);}
  }
}

/*void generateExpectedNote()
{
  long randNote = random(0, 100);
  expTimePressed = millis() + randNote;
  delay(randNote);
}*/

int doButtonRed()
{
  //Serial.println("buttonStateRed " + String(buttonStateRed));
  //Serial.println("lastButtonStateRed " + String(lastButtonStateRed));
  if (buttonStateRed == LOW) {
    //Serial.println("Red button was pressed");
    
    timePressed = millis() - startTime;
    //Serial.println(timePressed);
    buttonPressed = 1;
    lastButtonStateRed = buttonStateRed;
  }
  
  else { timePressed = -1;}
  return timePressed;
  
}

int doButtonWhite()
{
  if (buttonStateWhite == LOW)  
  {
    Serial.println("White button was pressed");
    
    timePressed = millis() - startTime;
    //Serial.println(timePressed);
    buttonPressed = 2;
    lastButtonStateWhite = buttonStateWhite;
  }
  
  else {timePressed = -1;}

  return timePressed;
}

int doButtonBlue()
{
  if (buttonStateBlue == LOW) {
    Serial.println("Blue button was pressed");
    
    timePressed = millis() - startTime;
    //Serial.println(timePressed);
    buttonPressed = 3;
    lastButtonStateBlue = buttonStateBlue;
  }
  
  else {timePressed = -1;}

  return timePressed;
}

void checkAccuracy(int expTimePressed,int timePressed, int expButtonPressed, int buttonPressed)
{
  Serial.println("expTimePressed: " + String(expTimePressed));
  Serial.println("timePressed: " + String(timePressed));
  //Serial.println("expButtonPressed: " + String(buttonPressed));
  //Serial.println("buttonPressed: " + String(buttonPressed));
  if (expTimePressed+1000 >= timePressed 
  && expTimePressed-1000 <= timePressed
  && expButtonPressed == buttonPressed)
  {
     accuracy++;
  }
  Serial.println("Accuracy:" + String(accuracy));
}


