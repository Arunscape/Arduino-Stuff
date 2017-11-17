/*
this code was modified from one of our submissions for Exercise 6

The original code can be found here:
https://drive.google.com/file/d/0BzukoWTLryYpeGhkUUM2S3RwYzg/view?usp=sharing
*/
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <SD.h>

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

//renamed this to not conflict with the tft variable in joy_cursor.cpp
Adafruit_ILI9341 tft_rest = Adafruit_ILI9341(TFT_CS, TFT_DC);

// different than SD
Sd2Card card;

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066

#define JOY_VERT  A1
#define JOY_HORIZ A0
#define JOY_SEL   2
#define JOY_CENTER   512
#define JOY_DEADZONE 64

//lines 21 to 45 are given code on eclass
// These constants are for the 2048 by 2048 map.
#define MAP_WIDTH 2048
#define MAP_HEIGHT 2048
#define LAT_NORTH 5361858l
#define LAT_SOUTH 5340953l
#define LON_WEST -11368652l
#define LON_EAST -11333496l

#define LCD_WIDTH 2048
#define LCD_HEIGHT 2048

// These functions convert between x/y map position and lat/lon
// (and vice versa.)
int32_t x_to_lon(int16_t x) {
    return map(x, 0, MAP_WIDTH, LON_WEST, LON_EAST);
}

int32_t y_to_lat(int16_t y) {
    return map(y, 0, MAP_HEIGHT, LAT_NORTH, LAT_SOUTH);
}

int16_t lon_to_x(int32_t lon) {
    return map(lon, LON_WEST, LON_EAST, 0, MAP_WIDTH);
}

int16_t lat_to_y(int32_t lat) {
    return map(lat, LAT_NORTH, LAT_SOUTH, 0, MAP_HEIGHT);
}

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};

// stores the 'pixel' coordinates of the selected restaurant
struct coords {
  int selectedRestX;
  int selectedRestY;
};

restaurant restBlock[8];

uint32_t globalBlockIndex;

struct RestDist {
  uint16_t index; // index of restaurant from 0 to NUM_RESTAURANTS-1
  uint16_t dist;  // manhattan distance to cursor position
};

RestDist rest_dist[NUM_RESTAURANTS];

void rest_setup() {
  //setup routine for the functions used in this file
  tft_rest.setRotation(3);

  Serial.print("Initializing SPI communication for raw reads...");
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }
  else {
    Serial.println("OK!");
  }
}

void getRestaurantFast(int restIndex, restaurant* restPtr){
  uint32_t blockNum = REST_START_BLOCK + restIndex/8;

  if (globalBlockIndex==restIndex/8){
    ;//do nothing, the block is already loaded in memory
  }

  else{
    //if the block is not already in the memory, read from the SD card until
    //the block that holds the restaurant at restindex is loaded
    while (!card.readBlock(blockNum, (uint8_t*) restBlock)) {
      Serial.println("Read block failed, trying again.");
    }
    globalBlockIndex = restIndex/8;
    //update the globalBlockIndex, which keeps track of which block is loaded

  }

  *restPtr = restBlock[restIndex % 8];

}

// computes manhattan distance between cursor and restaurant
void manhattanDistance(int cursorx, int cursory){
  int restx;
  int resty;

  for(int i=0; i<1066;i++){
    restaurant rest;
    getRestaurantFast(i, &rest);
    rest_dist[i].index=i;

    restx = lon_to_x(rest.lon);
    resty = lat_to_y(rest.lat);
    rest_dist[i].dist = abs(restx-cursorx) + abs(resty-cursory);
  }
}

//the ssort and swap_rest code below was given on eclass:
void swap_rest(RestDist *ptr_rest1, RestDist *ptr_rest2) {
  RestDist tmp = *ptr_rest1;
  *ptr_rest1 = *ptr_rest2;
  *ptr_rest2 = tmp;
}

// Selection sort
// rest_dist is an array of RestDist, from rest_dist[0] to rest_dist[len-1]
void ssort(RestDist *rest_dist, int len) {
  for (int i = len-1; i >= 1; --i) {
    // Find the index of furthest restaurant
    int max_idx = 0;
    for (int j = 1; j <= i; ++j) {
      if(rest_dist[j].dist > rest_dist[max_idx].dist) {
        max_idx = j;
      }
    }
    // Swap it with the last element of the sub-array
    swap_rest(&rest_dist[i], &rest_dist[max_idx]);
  }
}

// allows user to scroll through the restaurants in the menu
int scrollMenu(int selectedRest) {
  int yVal = analogRead(JOY_VERT);
  if (yVal < JOY_CENTER - JOY_DEADZONE) {
    selectedRest -= 1;
  }

  else if (yVal > JOY_CENTER + JOY_DEADZONE) {
    selectedRest += 1;
  }

  selectedRest = constrain(selectedRest,0,29);

  return selectedRest;
}

// display the 30 closest restaurants
void displayAllNames() {
  int selectedRest = 0;
  tft_rest.setCursor(0, 0); // where the characters will be displayed
  tft_rest.setTextWrap(false);
  for (int i = 0; i < 30; i++) {
    restaurant r;
    getRestaurantFast(rest_dist[i].index, &r);
    if (i != selectedRest) { // not highlighted
      tft_rest.setTextColor(0xFFFF, 0x0000); // white characters on black background
    } else { // highlighted
      tft_rest.setTextColor(0x0000, 0xFFFF); // black characters on white background
    }
    tft_rest.print(r.name);
    tft_rest.print("\n");
  }
  tft_rest.print("\n");
}

// update only the areas of the screen affected when the user scrolls through the restaurants
void updateName(int index, int selectedRest) {
  tft_rest.setCursor(0, 8*index);
  tft_rest.setTextWrap(false);
  restaurant r;
  getRestaurantFast(rest_dist[index].index,&r);
  if (index == selectedRest) {
    tft_rest.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  }

  if (index != selectedRest) {
    tft_rest.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  }

  tft_rest.print(r.name);
}


//the menu function that shows up when the user clicks on a point on the
//map , and displays the 30 closest restaurants in that area
coords menu(int CursorPixelX, int CursorPixelY){
  rest_setup();
  manhattanDistance(CursorPixelX,CursorPixelY);
  ssort(rest_dist,NUM_RESTAURANTS); //sort by distance

  tft_rest.fillScreen(0); //refresh the screen, make everything black first

  int selectedRest = 0;
  int buttonVal = 0;
  displayAllNames();

  while (!buttonVal) {
    //while user does not select a restaurant from the menu

    int oldSelectedRest = selectedRest;

    selectedRest = scrollMenu(selectedRest);

    // update the highlighted name when you move the cursor
    if (oldSelectedRest < selectedRest) {
      updateName(selectedRest,selectedRest);
      updateName(selectedRest-1,selectedRest);
    }

    else if (oldSelectedRest > selectedRest) {
      updateName(selectedRest,selectedRest);
      updateName(selectedRest+1,selectedRest);
    }

    buttonVal=  !digitalRead(2);
  }

  while(buttonVal){//if button is pressed, wait for it to be unpressed before returning to edmonton map
    buttonVal=  !digitalRead(2);
    delay(50);//added for consistency, because when the button is released, it floats for a little bit
  }

  // get the info from the selected restaurant
  restaurant r;
  getRestaurantFast(rest_dist[selectedRest].index,&r);

  coords selectedRestCoords;

  // store the coordinates of the restaurant as x,y values
  selectedRestCoords.selectedRestX = lon_to_x(r.lon);
  selectedRestCoords.selectedRestY = lat_to_y(r.lat);

//  selectedRestCoords.selectedRestX = constrain(selectedRestCoords.selectedRestX,0,LCD_WIDTH);
//  selectedRestCoords.selectedRestY = constrain(selectedRestCoords.selectedRestY,0,LCD_HEIGHT);

  delay(50);

  return selectedRestCoords;

}
