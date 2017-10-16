#include <Arduino.h>
#include "AutoKeyExchange.h"

//**This is a function that given on eclass
// Waits for a certain number of bytes on Serial3 or timeout
// @param nbytes: the number of bytes we want
// @param timeout: timeout period (ms); specifying a negative number
//                turns off timeouts (the function waits indefinitely
//                if timeouts are turned off).
// @return True if the required number of bytes have arrived.
bool wait_on_serial3( uint8_t nbytes, long timeout ) {
  unsigned long deadline = millis() + timeout;//wraparound not a problem
  while (Serial3.available()<nbytes && (timeout<0 || millis()<deadline))
  {
    delay(1); // be nice, no busy loop
  }
  return Serial3.available()>=nbytes;
}

//**This is a function that was given on eclass
// Writes an uint32_t to Serial3, starting from the least-significant
// and finishing with the most significant byte.
void uint32_to_serial3(uint32_t num) {
  Serial3.write((char) (num >> 0));
  Serial3.write((char) (num >> 8));
  Serial3.write((char) (num >> 16));
  Serial3.write((char) (num >> 24));
}

//**This is a function that was given on eclass
// Reads an uint32_t from Serial3, starting from the least-significant
// and finishing with the most significant byte.
uint32_t uint32_from_serial3() {
  uint32_t num = 0;
  num = num | ((uint32_t) Serial3.read()) << 0;
  num = num | ((uint32_t) Serial3.read()) << 8;
  num = num | ((uint32_t) Serial3.read()) << 16;
  num = num | ((uint32_t) Serial3.read()) << 24;
  return num;
}

// This function will exchange public keys over the serial monitor
// after a handshake is done, to make sure they are both ready to exchange
// keys. One Arduno acts as a client and sends conncection requests to a
// server. When a connection is establish, data exchange can begin.
uint32_t AutoKeyExchange(uint32_t pubkey) {

  uint32_t partnerPubKey;

  // set digital pin 13 to receive input
  pinMode(13,INPUT);

  // all the available states for the client are declared
  enum clientState { START, ACKWAIT, EXCHANGE_C };
  // all the available states for the server are declared
  enum serverState { LISTEN, KEYWAIT1, ACKWAIT1, KEYWAIT2, ACKWAIT2, EXCHANGE_S };

  // the client begins in the START state
  clientState currentState_C = START;


  // the Arduino that has ground connected to pin 13 is the client
  while (digitalRead(13) == LOW) {

    if (currentState_C == START) {
      // send the message CR(ckey), which the the character C, followed by
      // the 4 byte privkey
      Serial3.write('C');
      uint32_to_serial3(pubkey);
      // change the state to ACKWAIT
      currentState_C = ACKWAIT;
    }

    // if in ACKWAIT, and 5 bytes are received from Serial3
    if (currentState_C ==  ACKWAIT && wait_on_serial3(5,1000)) {
      // when the first byte read is the acknowledgment character 'A', then
      // the client will store the Server's public key from the remaining
      // bytes and send acknowledgment back
      if (Serial3.read() == 'A') {
        partnerPubKey = uint32_from_serial3();
        Serial3.write('A');
        currentState_C = EXCHANGE_C;
      }
      // if the first byte read is NOT 'A', go back to the start
      else {currentState_C = START;}
    }

    // if in ACKWAIT, and 5 bytes are NOT received from Serial3
    else if (currentState_C == ACKWAIT  && !wait_on_serial3(5,1000)) {
      // go back to the star
      currentState_C = START;
    }

    else if (currentState_C == EXCHANGE_C) {
      // if the exchange state is reached, break the loop and exit AutoKeyExchange
      // to continue on
      Serial.println("This Arduino is acting as the client...");
      break;
    }

  }

  // the server begins in the LISTEN state
  serverState currentState_S = LISTEN;

  // the Arduino that connects pin 13 to 5V is the server
  while (digitalRead(13) == HIGH) {

    // while listening, check if there's a byte available from Serial3
    if (currentState_S == LISTEN && Serial3.available() != 0) {

      // if the byte available from serial3 is the character 'C', proceed to
      // KEYWAIT1
      if (Serial3.read() == 'C') {
        currentState_S=KEYWAIT1;
      }
    }

    // if waiting for the key, and there are 4 bytes available in Serial3,
    // store these bytes as the partnerPubKey (its assumed that C is followed
    // by the key)
    if (currentState_S == KEYWAIT1 &&  wait_on_serial3(4,1000)) {
      partnerPubKey = uint32_from_serial3();
      // send an acknowledgment of key received
      Serial3.write('A');
      // send the user's own pubkey
      uint32_to_serial3(pubkey);
      // transition to ACKWAIT1
      currentState_S = ACKWAIT1;
    }

    // if 4 bytes are NOT received while waiting for the key, go back to LISTEN
    else if (currentState_S == KEYWAIT1 && !wait_on_serial3(4,1000)) {currentState_S=LISTEN;}

    // wait for 1 byte from serial3
    else if (currentState_S == ACKWAIT1 && wait_on_serial3(1,1000)) {
      // if that byte is the acknowledgment character, proceed to the EXCHANGE
      if (Serial3.read() == 'A') {currentState_S=EXCHANGE_S;}
      // if 'C' is received, proceed to the next KEYWAIT stage
      else if (Serial3.read() == 'C') {
        currentState_S=KEYWAIT2;
      }
    }

    // if no byte is received from Serial3, go back to LISTEN
    else if (currentState_S == ACKWAIT1 &&  !wait_on_serial3(1,1000)) {currentState_S=LISTEN;}

    // when waiting for the key, if 4 bytes are received, store them as
    // partnerPubKey
    else if (currentState_S == KEYWAIT2 && wait_on_serial3(4,1000)) {
      partnerPubKey = uint32_from_serial3();
      currentState_S = ACKWAIT2;
    }

    // when waiting for the key, if 4 bytes are NOT received , go back to LISTEN
    else if (currentState_S == KEYWAIT2 && !wait_on_serial3(4,1000)) {currentState_S=LISTEN;}

    // while waiting for acknowledgement, wait for 1 byte on Serial3
    else if (currentState_S == ACKWAIT2 && wait_on_serial3(1,1000)) {
      // if the byte is 'A', go to the EXCHANGE state
      if (Serial3.read() == 'A') {currentState_S=EXCHANGE_S;}
      // if the byte is 'C', go back to KEYWAIT
      else if (Serial3.read()== 'C') {currentState_S=KEYWAIT2;}
    }

    // while waiting for acknowledgement, go back to listen is no byte
    // is received
    else if (currentState_S == ACKWAIT2 && !wait_on_serial3(1,1000)) {
      currentState_S=LISTEN;
    }

    else if (currentState_S == EXCHANGE_S) {
      // if the exchange state is reached, break the loop and exit AutoKeyExchange
      // to continue on
      Serial.println("This Arduino is acting as the server...");
      break;
    }
  }

  return partnerPubKey;

}
