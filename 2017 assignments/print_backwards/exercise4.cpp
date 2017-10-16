//Arun Woosaree
#include <Arduino.h>
#include "string_processing.h"

void setup(){
  init();
  Serial.begin(9600);
}


int main(){
  setup();

  //manual tests by manual I mean directly calling the getStringLength() function
  //and having to manually verify that the numbers match in the serial monitor
  Serial.print(getStringLength("testing")); Serial.println(" expect 7");
  Serial.print(getStringLength("abcdefghijklmnopqrstuvwxyz")); Serial.println(" expect 26");
  Serial.print(getStringLength("hello")); Serial.println(" expect 5");
  Serial.print(getStringLength("12345678910")); Serial.println(" expect 11");
  Serial.print(getStringLength("\0")); Serial.println(" expect 0");

  Serial.println();

  //automatic tests comparing output of my function against the strlen function
  testgetStringLength("hello");
  testgetStringLength("testing");
  testgetStringLength("\0");
  testgetStringLength("sudo rm -rf /");
  testgetStringLength("hq39e327382g3849yh&$@@@^&");
  Serial.println();

  Serial.println("The text below should be reversed:");
  printBackward("woah");
  printBackward("hello");
  printBackward("racecar");
  //really big test:
  printBackward("\".sgge dah yehT\" ,deilper eH \"?klim fo snotrac 6 yub uoy did lleh eht yhW\" ,mih sksa efiw ehT .klim fo snotrac 6 htiw kcab semoc dnabsuh eht retal emit trohs A \"!6 teg ,sgge evah yeht fi dna ,klim fo notrac eno yub dna em rof gnippohs og esaelp uoy dluoC\" reenigne erawtfos a ,dnabsuh reh sksa efiw A");





  Serial.flush();
  return 0;
}
