/*
 Handles reading the restaurants from the SD card and sorting them
 */

#ifndef _REST_SD_H
#define _REST_SD_H

struct coords {
  int selectedRestX;
  int selectedRestY;
};

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};


void getRestaurantFast(int restIndex, restaurant* restPtr);

int manhattanDistance(int cursorx, int cursory);

coords menu(int CursorPixelX, int CursorPixelY);

int scrollMenu(int selectedRest);

#endif
