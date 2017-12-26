//Arun Woosaree
#include <Arduino.h>
#include "string_processing.h"

/* Computes and returns the length of a null-terminated string */
int getStringLength(const char* str){

  int index = 0; //variable to keep track of length/index
  char character= 'c'; // make character nonempty to prevent weird errors with it being empty on line 10

  while (character != '\0'){//while loop because to use a for loop, you need to know how many iterations
    character = str[index];
    //debugging code commented out below:
    // Serial.print(index);
    // Serial.print(" ");
    // Serial.println(character);
    index++;
  }
  return index-1;
}

void testgetStringLength(const char* str){
  //tests my own getStringLength function against the strlen one
  if(getStringLength(str)==strlen(str)){
    Serial.println("test passed");;
  }
  else{
    Serial.print("TEST FAILED, got ");
    Serial.print(getStringLength(str));
    Serial.print(" , expected: ");
    Serial.print(strlen(str));
    Serial.print(" for ");
    Serial.println(str);

  }
}

/* Prints a null-terminated string backwards to the serial port */
void printBackward(const char* str){
  int strlength = getStringLength(str); //get string length
  String backwardstr;
  for (int i=0; i<strlength; i++){
    //i will be used to index the new backwards string
    //strlength-i-1 will be used to index the old string because I started counting
    //from 1 for the value of strlength. Also I want to leave the null terminator
    //character where it is, that should stay at the end of the string

    backwardstr += str[strlength-i-1];
    //Serial.print(backwardstr[i]);
    }
    //Serial.println();
    Serial.println(backwardstr);
  //this seemed to print empty lines in serial monitor so instead, trying to
  //print one char at a time by doing a Serial.print for every character
  //in the for loop
}
