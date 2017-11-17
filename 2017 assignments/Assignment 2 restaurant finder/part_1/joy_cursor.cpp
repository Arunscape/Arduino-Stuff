/*
This file was modified from the solution for exercise 5 posted on eclass.
Our files work, but for consistency, we decided to use this file and build on it
*/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include "lcd_image.h"

#define tft_joy_DC 9
#define tft_joy_CS 10
#define SD_CS 6

// this was named tft_joy instead of tft to not confuse it with the other
// tft declarations
Adafruit_ILI9341 tft_joy = Adafruit_ILI9341(tft_joy_CS, tft_joy_DC);

// physical dimensions of the tft_joy display (# of pixels)
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240

// dimensions of the part allocated to the map display
#define MAP_DISP_WIDTH (DISPLAY_WIDTH - 48)
#define MAP_DISP_HEIGHT DISPLAY_HEIGHT

// width and height (in pixels) of the LCD image
#define LCD_WIDTH 2048
#define LCD_HEIGHT 2048
lcd_image_t yegImage = { "yeg-big.lcd", LCD_WIDTH, LCD_HEIGHT };

#define JOY_VERT  A1 // should connect A1 to pin VRx
#define JOY_HORIZ A0 // should connect A0 to pin VRy
#define JOY_SEL   2

#define JOY_CENTER   512
#define JOY_DEADZONE 64

#define CURSOR_SIZE 9

// smaller numbers yield faster cursor movement
#define JOY_SPEED 32

// the cursor position on the display, stored as the middle pixel of the cursor
int16_t cursorX, cursorY;

// upper-left coordinates in the image of the middle of the map of Edmonton
#define YEG_MIDDLE_X (LCD_WIDTH/2 - MAP_DISP_WIDTH/2)
#define YEG_MIDDLE_Y (LCD_HEIGHT/2 - MAP_DISP_HEIGHT/2)

int mapMiddleX = YEG_MIDDLE_X;
int mapMiddleY = YEG_MIDDLE_Y;

// forward declaration for drawing the cursor
void redrawCursor(int newX, int newY, int oldX, int oldY);

void joy_setup() {
  init();

  Serial.begin(9600);

	pinMode(JOY_SEL, INPUT_PULLUP);

	tft_joy.begin();

	Serial.print("Initializing SD card...");
	if (!SD.begin(SD_CS)) {
		Serial.println("failed! Is it inserted properly?");
		while (true) {}
	}
	Serial.println("OK!");

	tft_joy.setRotation(3);

  tft_joy.fillScreen(ILI9341_BLACK);

  // draws the centre of the Edmonton map, leaving the rightmost 48 columns black
	lcd_image_draw(&yegImage, &tft_joy, YEG_MIDDLE_X, YEG_MIDDLE_Y,
                 0, 0, MAP_DISP_WIDTH, MAP_DISP_HEIGHT);

  // initial cursor position is the middle of the screen
  cursorX = (MAP_DISP_WIDTH - CURSOR_SIZE)/2;
  cursorY = (MAP_DISP_HEIGHT - CURSOR_SIZE)/2;

  // draw the initial cursor
  redrawCursor(cursorX, cursorY, cursorX, cursorY);
}

// redraws the patch of edmonton over the older cursor position
// given by "oldX, oldY" and draws the cursor at its new position
// given by "newX, newY"
void redrawCursor(int newX, int newY, int oldX, int oldY) {

  // draw the patch of edmonton over the old cursor
  lcd_image_draw(&yegImage, &tft_joy,
                 mapMiddleX + oldX, mapMiddleY + oldY,
                 oldX, oldY, CURSOR_SIZE, CURSOR_SIZE);

  // and now draw the cursor at the new position
  tft_joy.fillRect(newX, newY, CURSOR_SIZE, CURSOR_SIZE, ILI9341_RED);
}

void scrollMap() {//function that allows the map to scroll when cursor hits edge of screen
  cursorX = (MAP_DISP_WIDTH - CURSOR_SIZE)/2;
  cursorY = (MAP_DISP_HEIGHT - CURSOR_SIZE)/2;

  // scroll left
  if (mapMiddleX < 0) {
    mapMiddleX = 0;
  }

  // right edge
  else if (mapMiddleX > LCD_WIDTH - MAP_DISP_WIDTH) {
    mapMiddleX = LCD_WIDTH - MAP_DISP_WIDTH;
  }

  // top edge
  if (mapMiddleY < 0) {
    mapMiddleY = 0;
  }

  // bottom edge
  else if (mapMiddleY > LCD_HEIGHT - MAP_DISP_HEIGHT) {
    mapMiddleY = LCD_HEIGHT - MAP_DISP_HEIGHT;
  }

  // redraw the map
  lcd_image_draw(&yegImage, &tft_joy,
     // upper left corner in the image to draw
     mapMiddleX, mapMiddleY,
     // upper left corner of the screen to draw it in
     0, 0,
     // width and height of the patch of the image to draw
     MAP_DISP_WIDTH, MAP_DISP_HEIGHT);
}

void processJoystick() {
  int xVal = analogRead(JOY_HORIZ);
  int yVal = analogRead(JOY_VERT);

  // copy the cursor position (to check later if it changed)
  int oldX = cursorX;
  int oldY = cursorY;

  // move the cursor, further pushes mean faster movement
  cursorX += (JOY_CENTER - xVal) / JOY_SPEED;
  cursorY += (yVal - JOY_CENTER) / JOY_SPEED;

  cursorX = constrain(cursorX, 0, MAP_DISP_WIDTH - CURSOR_SIZE);
  cursorY = constrain(cursorY, 0, MAP_DISP_HEIGHT - CURSOR_SIZE);

  // if cursor reaches right end of display
  if (cursorX == MAP_DISP_WIDTH - CURSOR_SIZE && mapMiddleX != LCD_WIDTH - MAP_DISP_WIDTH) {
    mapMiddleX += MAP_DISP_WIDTH/2;
    scrollMap();
  }

  // if cursor reachs left end of display
  else if (cursorX == 0 && mapMiddleX != 0) {
    mapMiddleX -= MAP_DISP_WIDTH/2;
    scrollMap();
  }

  // top
  if (cursorY == 0 && mapMiddleY != 0) {
    mapMiddleY -= MAP_DISP_HEIGHT/2;
    scrollMap();
  }

  // bottom
  else if (cursorY == MAP_DISP_HEIGHT - CURSOR_SIZE && mapMiddleY != LCD_HEIGHT - MAP_DISP_HEIGHT) {
    mapMiddleY += MAP_DISP_HEIGHT/2;
    scrollMap();
  }

  // redraw the cursor only if its position actually changed
  if (cursorX != oldX || cursorY != oldY) {
    redrawCursor(cursorX, cursorY, oldX, oldY);
  }

  delay(10);
}

//these functions calculate the cursor pixel position relative to the YEG image

int calcCursorPixelX() {
  return (mapMiddleX + cursorX);
}

int calcCursorPixelY() {
  return (mapMiddleY + cursorY);
}

void redrawMapFromMenu(int x, int y) {
  //redraws the map when user selects restauant from the menu

  //set the top left corner of the image to be drawn, so the restaurant is at the center
  mapMiddleX = x - MAP_DISP_WIDTH/2;
  mapMiddleY = y - MAP_DISP_HEIGHT/2;

  // these are conditions if the restaurant is outside of the map boundaries
  if (mapMiddleX < 0) {
    mapMiddleX = 0;
    cursorX = 0;

  }

  else if (mapMiddleX > LCD_WIDTH - MAP_DISP_WIDTH) {
    mapMiddleX = LCD_WIDTH - MAP_DISP_WIDTH;
    cursorX = MAP_DISP_WIDTH - CURSOR_SIZE;
  }

  else {
    cursorX = (MAP_DISP_WIDTH - CURSOR_SIZE)/2;
  }

  if (mapMiddleY < 0) {
    mapMiddleY = 0;
    cursorY = 0;
  }

  else if (mapMiddleY > LCD_HEIGHT - MAP_DISP_HEIGHT) {
    mapMiddleY = LCD_HEIGHT - MAP_DISP_HEIGHT;
    cursorY = MAP_DISP_HEIGHT - CURSOR_SIZE;
  }

  else{
    cursorY = (MAP_DISP_HEIGHT - CURSOR_SIZE)/2;
  }

  tft_joy.fillScreen(ILI9341_BLACK);

  lcd_image_draw(&yegImage, &tft_joy,
     // upper left corner in the image to draw
     mapMiddleX, mapMiddleY,
     // upper left corner of the screen to draw it in
     0, 0,
     // width and height of the patch of the image to draw
     MAP_DISP_WIDTH, MAP_DISP_HEIGHT);

  // -100,-100 is chosen at the old cursor postion, so that it is never the same
  // as the cursor we want to draw
  redrawCursor(cursorX,cursorY, -100,-100);
}
