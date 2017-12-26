/*
Arun Woodaree
A lot of this code was given in class, the assignment required us to
modify the code such that the cursor does not leave a black trail,
variable cursor speed, the cursor shouldn't flash while not moving,
and the cursor should not be able to move off the edge of the map
*/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include "lcd_image.h"

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define YEG_SIZE 2048

lcd_image_t yegImage = { "yeg-big.lcd", YEG_SIZE, YEG_SIZE };

#define JOY_VERT  A1 // should connect A1 to pin VRx
#define JOY_HORIZ A0 // should connect A0 to pin VRy
#define JOY_SEL   2

#define JOY_CENTER   512
#define JOY_DEADZONE 64

#define CURSOR_SIZE 9

#define yegMiddleX YEG_SIZE/2 - (DISPLAY_WIDTH - 48)/2
#define yegMiddleY YEG_SIZE/2 - DISPLAY_HEIGHT/2

// the cursor position on the display
int cursorX, cursorY;

// forward declaration for redrawing the cursor
void redrawCursor(uint16_t colour);

void setup() {
  init();

  Serial.begin(9600);

	pinMode(JOY_SEL, INPUT_PULLUP);

	tft.begin();

	Serial.print("Initializing SD card...");
	if (!SD.begin(SD_CS)) {
		Serial.println("failed! Is it inserted properly?");
		while (true) {}
	}
	Serial.println("OK!");

	tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);

  // draws the centre of the Edmonton map, leaving the rightmost 48 columns black

  //moving the yegmiddle variables to #define at the beginning so I can access it
  //from any function
  // int yegMiddleX = YEG_SIZE/2 - (DISPLAY_WIDTH - 48)/2;
	// int yegMiddleY = YEG_SIZE/2 - DISPLAY_HEIGHT/2;

	lcd_image_draw(&yegImage, &tft, yegMiddleX, yegMiddleY,
                 0, 0, DISPLAY_WIDTH - 48, DISPLAY_HEIGHT);

  // initial cursor position is the middle of the screen
  cursorX = (DISPLAY_WIDTH - 48)/2;
  cursorY = DISPLAY_HEIGHT/2;

  redrawCursor(ILI9341_RED);
}

void redrawCursor(uint16_t colour) {
  tft.fillRect(cursorX - CURSOR_SIZE/2, cursorY - CURSOR_SIZE/2,
               CURSOR_SIZE, CURSOR_SIZE, colour);
}

//new function based on redrawCursor that should redraw the map where the cursor was
void redrawMap(){
  lcd_image_draw(&yegImage, &tft,
  		    yegMiddleX+cursorX-CURSOR_SIZE/2, yegMiddleY+cursorY-CURSOR_SIZE/2, //position on image
  		    cursorX - CURSOR_SIZE/2, cursorY - CURSOR_SIZE/2, //position on screen
  		    CURSOR_SIZE, CURSOR_SIZE) ;//width and height of image to draw
}


//function modified to match assignment requirements
void processJoystick() {
  int xVal = analogRead(JOY_HORIZ);
  int yVal = analogRead(JOY_VERT);
  int buttonVal = digitalRead(JOY_SEL);

  //adding code to stop the arduino from refreshing the cursor while it's not moving
  //by getting it stuck in a while loop until user moves it out of the deadzone
  while(yVal < JOY_CENTER+JOY_DEADZONE and yVal > JOY_CENTER-JOY_DEADZONE and xVal < JOY_CENTER+JOY_DEADZONE and xVal > JOY_CENTER-JOY_DEADZONE){
    //keep reading the values of the x and y position
    xVal = analogRead(JOY_HORIZ);
    yVal = analogRead(JOY_VERT);
    //int buttonVal = digitalRead(JOY_SEL);
  }

  //redrawCursor(ILI9341_BLACK);
  //don't want the cursor to leave a black trail anymore, we want to redraw the
  //part of the map left behind

  //redraw the map where the cursor was
  redrawMap();


  // now move the cursor

  //speed varies depending on the xval or yval's absolute
  //distance from the center, and divide by 50 , because the max difference is
  //pretty large (1023-512), and much bigger than the amount of pixels on the screen,
  //so a max cursor change of (1023-512)/50 ~= 10 seems reasonable
  if (yVal < JOY_CENTER - JOY_DEADZONE) {
    cursorY -= (JOY_CENTER-yVal)/50; // decrease the y coordinate of the cursor
  }
  else if (yVal > JOY_CENTER + JOY_DEADZONE) {
    cursorY += (yVal-JOY_CENTER)/50;
  }

  //constrain the cursor to the screen
  cursorY = constrain(cursorY,0+CURSOR_SIZE/2,DISPLAY_HEIGHT-CURSOR_SIZE/2-1);
  //had to add -1 to the upper bound, probably because CURSOR_SIZE is an odd number,
  //so some rounding occurs. Another possible fix would be to make CURSOR_SIZE
  //an even number like 10 or 8
  //otherwise, one row of pixels could go off screen

  // remember the x-reading increases as we push left
  if (xVal > JOY_CENTER + JOY_DEADZONE) {
    cursorX -= (xVal-JOY_CENTER)/50;
  }
  else if (xVal < JOY_CENTER - JOY_DEADZONE) {
    cursorX += (JOY_CENTER-xVal)/50;
  }

  //constrain the cursor to the screen
  cursorX = constrain(cursorX,0+CURSOR_SIZE/2,DISPLAY_WIDTH-48-CURSOR_SIZE/2-1);
  //had to add -1 to the upper bound, probably because CURSOR_SIZE is an odd number,
  //so some rounding occurs. Another possible fix would be to make CURSOR_SIZE
  //an even number like 10 or 8
  //otherwise, one column of pixels could go off screen


  // also, draw a black square at the old cursor position before
  // moving and drawing a red square

  redrawCursor(ILI9341_RED);

  delay(20);
}

int main() {
	setup();

  while (true) {
    processJoystick();
  }

	Serial.end();
	return 0;
}
