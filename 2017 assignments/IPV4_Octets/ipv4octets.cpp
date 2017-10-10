#include <Arduino.h>

void setup(){
  init();
  Serial.begin(9600);
}

uint8_t getOctet(uint32_t ipAddress, uint8_t octet){
/*
ipAddress >> 8*octet will shift the requested octet in binary to the rightmost
octet. E.g. the leftmost octet is octet 3, and if requested, that has to be
shifted 24 bits, or 8*3 bits to the right. Once that is done, we are only
concerned with the rightmost octet. In part 1 of assignment 1, we were able to
get the least significant bit by adding & 1 to the end of the line of code.
Here, we didn't need to worry about which base we were using because 1 is
represented the same way in base 2 and higher

This time, we need to fetch the 8 bits on the right side,
from ece210 we learned 1111 1111 can be represented in the
octal system as 377, and in hexadecimal as FF. This can be represented in many
ways in C++, for example: (this website helped: http://www.cplusplus.com/doc/tutorial/constants/ )

  0b11111111 in binary
  0377 in octal
  0xFF in hexadecimal
   255 in decimal

I'll be using hexadecimal simply because 0xFF looks cool in the code, and it's
compact compared to 0b11111111. Also, typing the full binary number gives lots
of opportunities for typing in the wrong number and not noticing the typo
*/
  return (ipAddress >> 8*octet) & 0xFF;
}

void AdvancedtestGetOctet(){
  for (int i=0; i<1000; i++){
    //generate some pseudo random ip addresses and test them
    //using a for loop because I'm too lazy to write repetitive code to test
    //similar cases, and also now I can easily run 1000 tests (or more), so if
    //there is an error, I'm very likely to find it
    //edit: this actually turned out to be more compicated than I thought
    //but still fun to make, haha

    int oct[4]; //generate 4 random numbers tothem to make the ip

    oct[3] = random(256);
    oct[2] = random(256);
    oct[1] = random(256);
    oct[0] = random(256);

    Serial.print("Generated ip: "); //let user know what ip was generated
    Serial.print(oct[3]);
    Serial.print(".");
    Serial.print(oct[2]);
    Serial.print(".");
    Serial.print(oct[1]);
    Serial.print(".");
    Serial.print(oct[0]);
    Serial.println();


    uint32_t testip = 0; //generate a decimal representation of the testip

    for (int j = 3; j>-1; j--){//bitshift to concat integers
      testip = (testip << 8) + oct[j];
    }

    Serial.print("Decimal value: ");
    Serial.print(testip);
    Serial.println();
    Serial.println();

    for (int k=0; k<4; k++){

      Serial.print("Octet ");
      Serial.print(k);
      Serial.print(": ");
      int tmp = getOctet(testip,k);
      Serial.println(tmp);

      if (oct[k] != tmp){//print error if test failed
        Serial.print("ERROR: OCTET ");
        Serial.print(k);
        Serial.println(" NOT EQUAL");
      }



    }
    Serial.println();
    Serial.println();
  }
}

void testGetOctet(uint32_t ipAddress, uint8_t octet, int expected_result){

  if (getOctet(ipAddress,octet) != expected_result){
    //if octet does not math produce error
    Serial.print("ERROR!: ");
    Serial.print(ipAddress);
    Serial.print(" Octet: ");
    Serial.print(octet);
    Serial.print(" expected: ");
    Serial.print(expected_result);
    Serial.print(" Got: ");
    Serial.println(getOctet(ipAddress,octet));
  }

  else{
    Serial.println("Test passed.");
  }

}

uint32_t readSerial() {
  // this function is modified from a previous assignment where I had to
  //read from the serial monitor also (Assignment 1 part 1)

	//array has room for 32 bits and null operator. The max 32bit unsigned integer
  // is 4,294,967,295, which is 10 digits long. Add one more char space for \0
	char str[11];

	int index = 0;
	while (index < 10 ){//0-9 because this is 1 less than our string length
		// if something is waiting to be read
		if (Serial.available() > 0) {
			char byteRead = Serial.read();
      Serial.print(byteRead);

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

  Serial.println();
	//convert the string to a 32bit integer and return this value
	return atol(str);
}

int main(){
  setup();

  //good test cases. If you want more tests, uncomment the
  //AdvancedtestGetOctet() function. That function pseudo randomly generates
  //ip addresses and checks it with the testGetOctet() function. I say
  //"pseudo random" because it calls the random() function which isn't
  //truly random, but it serves the purpose of testing a ton of different ip
  //addresses
  testGetOctet(2130706433,3,127); //there is no place like 127.0.0.1
  testGetOctet(1249763400,1,224); //74.125.224.72  ip address belonging to google
  testGetOctet(3494108894,2,67); //208.67.222.222  openDNS
  testGetOctet(2539995532,0,140); //151.101.65.140 ip when I pinged reddit.com

  //tests that should fail
  testGetOctet(1162167621,1,96); //69.69.69.69
  testGetOctet(4294967040,2,552); //255.255.255.0

  //AdvancedtestGetOctet works like a charm, but takes a long time to do its
  //tests. Feel free to check it out by uncommenting line 171!

  //AdvancedtestGetOctet();

  Serial.println("Done tests, starting program...");
  Serial.println();



  while (true){ //allows user to enter ip in decimal form and function prints octets
    Serial.print("Enter an IP address as a 32 bit unsigned decimal number: ");
    uint32_t dec_ip = readSerial();// get decimal ip from user input

    //print the value of each octet
    for (int i=0; i<4; i++){
      Serial.print("Octet ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(getOctet(dec_ip,i));
    }
    Serial.println();


  }



  Serial.flush();
  return 0;
}
