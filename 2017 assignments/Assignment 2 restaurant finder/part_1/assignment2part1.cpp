#include <Arduino.h>
#include "joy_cursor.h"
#include "rest_sd.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <SD.h>

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

//holds pixel coords of the selected restaurant
coords selectedRestCoords;

void setup(){
  init();
}

int main(){
  setup();
  joy_setup(); //setup function in joy_cursor.cpp

  while(true){

    int buttonVal = digitalRead(2);//check if the joystick button is pressed

    if(buttonVal == 0){//if joystick pressed, go to restaurant menu
      //restaurant menu stuff
      selectedRestCoords = menu(calcCursorPixelX(),calcCursorPixelY());
      redrawMapFromMenu(selectedRestCoords.selectedRestX, selectedRestCoords.selectedRestY);
    }
    processJoystick();
  }

  Serial.end();
  return 0;
}
