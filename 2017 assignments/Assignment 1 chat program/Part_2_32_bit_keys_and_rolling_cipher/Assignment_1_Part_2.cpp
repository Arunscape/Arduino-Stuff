/* Names:
Arun Woosaree
Tamara Bojovic
*/

#include <Arduino.h>
//contains the function to generate the private key,
//and the powmodfast() function developed in class, which is used
//to generate the public and shared keys
//also contains the rolling cipher function which was given on eclass

#include "KeyGen.h"

//function to do the automatic key exchange. This was relatively big, so
//we made it into a new file
#include "AutoKeyExchange.h"

void setup(){
  init();
  Serial.begin(9600);//begin Serial activity
  Serial3.begin(9600);
}

int main(){
  setup();

  const uint32_t p = 2147483647;
  const uint32_t g = 16807;

  Serial.println("Welcome to the Arduino Chat!");
  Serial.println("--------------------------------------------------------------");
  Serial.println("Reminder: Do not share your private or shared keys!");
  Serial.println();
  uint32_t privateKey = privKeyGen(); //get private key
  Serial.print("Your private key is: ");
  Serial.println(privateKey);

  uint32_t local_pubkey = powModFast(g,privateKey,p); //get local public key
  Serial.print("Your public key is: ");
  Serial.println(local_pubkey);

  //commence handshake for the auto key exchange
  uint32_t partnerpubkey = AutoKeyExchange(local_pubkey);
  Serial.print("Partner's public key is: ");
  Serial.println(partnerpubkey);

  uint32_t sharedkey = powModFast(partnerpubkey, privateKey, p);//compute the shared key
  Serial.print("Your shared key is: ");
  Serial.println((uint32_t)sharedkey);

  //initially, for both sequences, we start with the sharedkey
  uint32_t encrypt_key = sharedkey;
  uint32_t decrypt_key = sharedkey;

  Serial.println("You may begin chatting!");
  Serial.println();
  Serial.println();


  while (true){
    //the following code is modified from the chat() function we had in part 1
    while (Serial3.available() == 0 && Serial.available() == 0){}
    // execution of chat is delayed until something is read from the user's
    // arduino or the other arduino

    if (Serial.available() != 0){

      // read a char from the serial monitor
      uint8_t	byteRead = Serial.read();
      // encrypt the char that was read using encrypt_key
      // cast encrypt_key to uint8_t before encryption
      uint8_t encryptedChar = byteRead ^ ((uint8_t) encrypt_key);

      // if the user presses enter (13 is the carriage return code in ASCII)
      if (byteRead == 13 ){
        // print a new line on user's serial monitor
        Serial.println();
        // encrypt the carriage return character then send it
        Serial3.write(encryptedChar);

        //get new key
        encrypt_key = next_key(encrypt_key);
        // encrypt the new line character, then send it
        // 10 is the newline code in ASCII
        Serial3.write(10 ^ ((uint8_t) encrypt_key));
      }

      else {
        // send the encrypted character to partner
        Serial3.write(encryptedChar);
        // echo message on user's own serial monitor
        Serial.write(byteRead);
      }

      //generate new encrypt_key
      encrypt_key = next_key(encrypt_key);
    }

    if (Serial3.available() != 0) {
      // read the encrypted character sent by partner
      uint8_t encryptedByteRead = Serial3.read();
      // decrypt the encrypted message
      uint8_t decryptedChar =  encryptedByteRead ^ ((uint8_t) decrypt_key);
      // print the decrypted message
      Serial.write(decryptedChar);
      decrypt_key = next_key(decrypt_key);

    }
  }


}
