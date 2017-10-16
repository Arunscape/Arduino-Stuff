/* Names:

Arun Woosaree
Tamara Bojovic

*/

#include <Arduino.h>


void setup() {
	init();
	Serial.begin(9600); //begin serial activity
	Serial3.begin(9600); //partner is connected to serial3
}

uint16_t privKeyGen() {
	//setup pin1 as an input. It's floatng so the voltage read will fuctuate
  //we use this to randomly generate numbers
  const int analogPin = 1;
  pinMode(analogPin, INPUT);

  uint16_t key=0; //declaring the variable where the key will be stored

	for (int i=0; i<16; i++){//get 16 random bits to make the key
    int bitread = analogRead(analogPin) & 1 ;//gets the least significant bit form the voltage read by analogRead
    //bitshift to the left so we can add the next bit to the right side
		key = key << 1;
		//add next bit to tHe right side
		key = key + bitread;

		delay(50);

	}

  return key;

}

uint32_t powMod(uint32_t a, uint32_t b, uint32_t m) {
	// multiply a by itself b times, keeping the intermediate values mod m
	// each step
	// uint32_t is used as to not overflow

	a=a%m;
	uint32_t result = 1 % m; // even initialize to be mod m
	for (int i=0; i<b; i++) {
		result = (result*a) % m;
	}

	return result;

}

uint16_t readPartnerKey() {
	// this function will read the public key that the user enter in the
	// serial monitor and store it in a char array, to be converted to an integer

	// array has room for 16 bits and null operator
	char str[17];

	int index = 0;
	while (index < 16) {//16 because this is 1 less than our string length
		// if something is waiting to be read
		if (Serial.available() > 0) {
			char byteRead = Serial.read();

			if (byteRead == '\r') {
				// if the user presses enter, break the loop to confirm end the string
				break;
			}
			else {
				// add the char read to the str array
				str[index] = byteRead;
				index += 1;
			}
		}
	}
	// add the null operator to complete the string
	str[index] = '\0';

	//convert the string to an integer and return this value
	return atoi(str);
}

uint16_t getSharedKey(uint16_t privkey, uint16_t pubkey) {
	// these are one letter variables and therefore not that great, but
	// it's how we defined the variables in the Diffie-Hellman worksheet,
	// so for consistency, we're sticking to the one letter variable names
	const uint32_t p = 19211;
	const uint32_t g = 6;

	Serial.print("Enter your partner's public key: ");
	uint16_t partnerKey = readPartnerKey();
	Serial.println();

	Serial.print("You entered: ");
	Serial.print(partnerKey);
	Serial.println();


	uint16_t sharedKey = powMod(partnerKey, privkey, p); //use formula B^(a) mod p
	return sharedKey;
}



void chat(uint16_t sharedKey){
	// function that allows the user to have an encrypted chat

	while (Serial3.available() == 0 && Serial.available() == 0){}
	// execution of chat is delayed until something is read from the user's
	// arduino or the other arduino

	if (Serial.available() != 0){

		// read a char from the serial monitor
		uint8_t	byteRead = Serial.read();
		// encrypt the char that was read using the sharedKey
		// cast the uint16_t sharedKey to uint8_t before encryption
		uint8_t encryptedChar = byteRead ^ ((uint8_t) sharedKey);

		// if the user presses enter (13 is the carriage return code in ASCII)
		if (byteRead == 13 ){
			// print a new line on user's serial monitor
			Serial.println();
			// encrypt the carriage return character then send it
			Serial3.write(encryptedChar);
			// encrypt the new line character, then send it
      // 10 is the newline code in ASCII
			Serial3.write(10 ^ ((uint8_t) sharedKey));
		}

		else {
			// send the encrypted character to partner
			Serial3.write(encryptedChar);
			// echo message on user's own serial monitor
			Serial.write(byteRead);
		}
	}

	if (Serial3.available() != 0) {
		// read the encrypted character sent by partner
		uint8_t encryptedByteRead = Serial3.read();
		// decrypt the encrypted message
		uint8_t decryptedChar =  encryptedByteRead ^ ((uint8_t) sharedKey);
		// print the decrypted message
		Serial.write(decryptedChar);

	}

}

int main() {
	setup();

	// these are one letter variables and therefore not that great, but
	// it's how we defined the variables in the Diffie-Hellman worksheet,
	// so for consistency, we're sticking to the one letter variable names
	const uint32_t p = 19211;
	const uint32_t g = 6;

	Serial.println("WARNING! DO NOT SHARE YOUR PRIVATE OR SHARED KEYS!");
	Serial.println();

	//generate private key
	uint16_t privkey = privKeyGen();
	Serial.print("Your private key is: ");
	Serial.print(privkey);
	Serial.println();

	//compute public key from private key
	uint16_t pubkey = powMod(g,privkey,p); //use formula g^(a) mod P
	Serial.print("Your public key is: ");
	Serial.print(pubkey);
	Serial.println();

	//exchange public keys and compute shared key
	uint16_t sharedKey = getSharedKey(privkey,pubkey);
	Serial.print("Your shared key is: ");
	Serial.print(sharedKey);
	Serial.println();
	Serial.println();

	// begin the encrypted chat
	Serial.println("Welcome to the chat! You may begin typing. Press CTRL + A then X to exit.");
	Serial.println();

	while (true) {
		chat(sharedKey); //users can now chat
	}

	Serial3.flush();
	Serial.flush();
	return 0;
}
