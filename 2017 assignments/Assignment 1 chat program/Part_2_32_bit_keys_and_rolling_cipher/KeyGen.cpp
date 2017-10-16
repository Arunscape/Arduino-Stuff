#include <Arduino.h>
#include "KeyGen.h"

uint32_t privKeyGen() { //generate private key
	//setup pin1 as an input. It's floatng so the voltage read will fuctuate
  //we use this to randomly generate numbers
  const int analogPin = 1;
  pinMode(analogPin, INPUT);

  uint32_t key=0; //declaring the variable where the key will be stored

	for (int i=0; i<32; i++){//get 32 random bits to make the key
    int bitread = analogRead(analogPin) & 1 ;//gets the least significant bit form the voltage read by analogRead
    //bitshift to the left so we can add the next bit to the right side
		key = key << 1;
		//add next bit to tHe right side
		key = key + bitread;

		delay(50);

	}

  return key;

}

uint32_t mulMod(uint32_t a, uint32_t b, uint32_t m) {
	uint32_t result = 0;

	for (int i = 0; i < 32; i++) {//iterate 32 times
		// if the i'th bit of a is 1
		// shifting moves the i'th bit to position 0, then & 1 isolates this bit
		if (((a>>i)&1) == 1) {
      //capture the ith bit, and if it's 1 , add b to result and mod it

      result = ((result + b)%m)%m;
		}

		b = (b*2) %m; //multiply b by 2 at each step
	}

	return result;

}

//the powModFast() function was developped in class. It's functionally the same
//as what we were using before, but this one works in a few seconds, while the
//old function used in part 1 would take days for a large number
//we modified this function to call a new function we made called mulMod, which
//does repeated multiplication to not overflow 32 bits
uint32_t powModFast(uint32_t a, uint32_t b, uint32_t m) {
	// compute ap[0] = a
	// ap[1] = ap[0]*ap[0]
	// ...
	// ap[i] = ap[i-1]*ap[i-1] (all mod m) for i >= 1

	uint32_t result = 1%m;
	uint32_t sqrVal = a%m; //stores a^{2^i} values, initially 2^{2^0}
	uint32_t newB = b;

	// LOOP INVARIANT: statements that hold throughout a loop
	//                 with the goal of being convinced the loop works
	// statements: just before iteration i,
	//               result = a^{binary number represented the first i bits of b}
	//               sqrVal = a^{2^i}
	//               newB = b >> i
	while (newB > 0) {
		if (newB&1) { // evalutates to true iff i'th bit of b is 1

			result = mulMod(result,sqrVal,m);//repeated multiplication
		}
		sqrVal = mulMod(sqrVal,sqrVal,m);
		newB = (newB>>1);
	}

	// upon termination: newB == 0
	// so b >> i is 0
	// so result a^{binary number represented the first i bits of b} = a^b, DONE!

	// # iterations ~ log_2 b ~ # of bits to write b
	// running time = O(log b)
	// NOTATION: we write O(some function) to express how the running times
	//           grows as a function of the input

	return result;
}

//this function was given in eclass, it calculates the next_key
//in our rolling ciper while chatting
uint32_t next_key(uint32_t current_key) {
  /** Implements the Park-Miller algorithm with 32 bit integer arithmetic
   * @return ((current_key * 48271)) mod (2^31 - 1);
   * This is linear congruential generator, based on the multiplicative
   * group of integers modulo m = 2^31 - 1.
   * The generator has a long period and it is relatively efficient.
   * Most importantly, the generator's modulus is not a power of two
   * (as is for the built-in rng),
   * hence the keys mod 2^{s} cannot be obtained
   * by using a key with s bits.
   * Based on:
   * http://www.firstpr.com.au/dsp/rand31/rand31-park-miller-carta.cc.txt
   */
  const uint32_t modulus = 0x7FFFFFFF; // 2^31-1
  const uint32_t consta = 48271;  // we use that consta<2^16
  uint32_t lo = consta*(current_key & 0xFFFF);
  uint32_t hi = consta*(current_key >> 16);
  lo += (hi & 0x7FFF)<<16;
  lo += hi>>15;
  if (lo > modulus) lo -= modulus;
  return lo;
}
