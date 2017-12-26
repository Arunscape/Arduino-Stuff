/*
Arun Woosaree
the bulk of code was developed in class. The getRestaurantFast() function was added
and a few minor modifications were made. The modifications are highlighted with
NOTE in the comments where I explain the tweaks. Some original bits have just
been completely commented out because they printed unwanted messages to the
serial monitor


Acknowledgements:
Christian Lo and Zhijie Shen helped me out with understanding pointers

The original code can be found here:
https://drive.google.com/file/d/0BzukoWTLryYpeGhkUUM2S3RwYzg/view?usp=sharing
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// different than SD
Sd2Card card;

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};

//NOTE moved this variable to be a global variable, so that the program does not
//need to allocate another 512 bytes temporarily, it can just read from the
//globally declared varible now. I think that this makes the program more
//memory efficient, as well
restaurant restBlock[8];

//NOTE new variable to keep track of which block is currently loaded in memory,
//so that the arduino doesn't have to re-read from the sd card if the current
//block is already located in memory
uint32_t globalBlockIndex;

void setup() {
  init();
  Serial.begin(9600);

  // including this seems to fix some SD card readblock errors
  // (at least on the old display)
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  // The following initialization (commented out) is
  // not needed for just raw reads from the SD card, but you should add it later
  // when you bring the map picture back into your assignment
  // (both initializations are required for the assignment)

  // initialize the SD card
  /*
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }
  else {
    Serial.println("OK!");
  }
  */

  // initialize SPI (serial peripheral interface)
  // communication between the Arduino and the SD controller

  Serial.print("Initializing SPI communication for raw reads...");
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }
  else {
    Serial.println("OK!");
  }
}

void testStructAlignment() {
  restaurant rest;

  if ((int) &rest != (int) &rest.lat) {
    Serial.println("latitude not at start");
  }
  else if (((int) &rest) + 4 != (int) &rest.lon) {
    Serial.println("longitude not 4 bytes from the start");
  }
  else if (((int) &rest) + 8 != (int) &rest.rating) {
    Serial.println("rating not 8 bytes from the start");
  }
  else if (((int) &rest) + 9 != (int) &rest.name) {
    Serial.println("name not 9 bytes from the start");
  }
  else {
    Serial.println("restaurant struct alignment is ok");
  }
}

// read the restaurant at position "restIndex" from the card
// and store at the location pointed to by restPtr
void getRestaurant(int restIndex, restaurant* restPtr) {
  // calculate the block containing this restaurant
  uint32_t blockNum = REST_START_BLOCK + restIndex/8;
  restaurant restBlock[8];

  //Serial.println(blockNum);

  // fetch the block of restaurants containing the restaurant
  // with index "restIndex"
  while (!card.readBlock(blockNum, (uint8_t*) restBlock)) {
    Serial.println("Read block failed, trying again.");
  }
  // Serial.print("Loaded: ");
  // Serial.println(restBlock[0].name);

  *restPtr = restBlock[restIndex % 8];
}

//NOTE modified version of getRestaurant() function.
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

int main() {
  setup();
  testStructAlignment();

  // can initialize a struct by specifying the value of its component
  // in the same order the components are listed in the struct declaration
  // restaurant rest = { 123, 456, 9, "Tim Horton's" };
  //
  // Serial.println(rest.lat);
  // Serial.println(rest.lon);
  // Serial.println(rest.rating);
  // Serial.println(rest.name);
  //
  // rest.lat = -747;
  // Serial.println(rest.lat);
  //
  // Serial.print("size of the restaurant struct: ");
  // Serial.println(sizeof(rest));
  //
  // // copies over all bytes of rest into rest2, including the string
  // restaurant rest2 = rest;
  // Serial.println(rest2.name);
  //
  // // copy the characters of the string "Subway" into the 55 characters
  // // of the rest2 struct
  // strcpy(rest2.name, "Subway");
  // Serial.println(rest2.name);

  // now start reading restaurant data, let's do the first block now

  // restaurant restBlock[8]; // 512 bytes in total: a block
  //NOTE commented out above line to make the variable global, so it can be
  //accessed using the getRestaurantFast function

  Serial.println();
  Serial.println("Now reading restaurants");

  // expects a uint8_t pointer to the start of the 512 bytes
  // that will receive the block
  card.readBlock(REST_START_BLOCK, (uint8_t*) restBlock);

  // print all restaurant names in this block
  // for (int i = 0; i < 8; ++i) {
  //   Serial.println(restBlock[i].name);
  // }
  //
  // Serial.println();
  // Serial.println("Fetching the first 20 restaurants");
  restaurant rest3;
  unsigned long time=millis(); //variable to keep track of time

  for (int i = 0; i < 1066; ++i) { //test the getRestaurant() function
    getRestaurant(i, &rest3);
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(rest3.name);
  }

  //print the time it took to read all the restaurants using getRestaurant()
  time = millis()-time; //the time it took to run
  Serial.print("Time using getRestaurant(): ");
  Serial.print(time);
  Serial.println();

  time =millis();
  for (int i = 0; i < 1066; ++i) {
    getRestaurantFast(i, &rest3);
  }

  //print the time it took to read all the restaurants using getRestaurantFast()
  time=millis()-time; //time taken to run
  Serial.print("Time using getRestaurantFast(): ");
  Serial.print(time);
  Serial.println();



  Serial.end();
  return 0;
}
