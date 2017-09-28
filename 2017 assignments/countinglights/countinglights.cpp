/*
Arun Woosaree

This is a simple program, which can count in binary using an array of 5 LEDs
It can also count backwards, and if you try to go below 0, it reverts back to showing
the number 31 in binary. (ie. if all the lights are off, and you try to decrement,
all the LEDs will turn on, and if you choose to keep hitting the minusButton,
it will keep counting backwards)
*/

#include <Arduino.h>

// assign pins to LEDs and button
const int LEDpins[5] = {8,9,10,11,12}; //I got the idea for declaring the pin numbers like this from: https://eclass.srv.ualberta.ca/pluginfile.php/3770936/mod_resource/content/6/section/lab_Lights(Transcribed).htm#SEC_8.3
const int plusButton = 7; //increment button
const int minusButton = 6; //decrement button

void setup(){
	init(); //low level functions required for Arduino

	// initialize pins
	// using a simple for loop so I don't have to write 5 redundant lines of code
	// for setting the LED pins to be an output
	for (int i = 0; i < 5 ; i++){
		pinMode(LEDpins[i],OUTPUT);
	}

	// init button pins and turn on internal pullup resistor to get reliable input
	// from the buttons
	pinMode(plusButton,INPUT);
	digitalWrite(plusButton,HIGH);

	pinMode(minusButton,INPUT);
	digitalWrite(minusButton, HIGH);
}


//this function makes the arduino wait for the user to lift their
//finger off the button, that way you can't cycle through the LEDs
//turning on and off by holding the button down, the button must be
//let go before the next LED state change

/* while(digitalRead(button)==LOW){continue;} did not end up working, because
the condition was not being checked since the code in the while
loop did nothing. Googling the issue suggested I needed to create a
variable with the volatile type, however,I found an easier fix by
messing around and figured out that I could use an infinite while
loop, and use break to exit it.*/


void WaitforReleaseMinus(){
	while (true){
		delay(10);
		/*small delay for more reliable input, since sometimes, the button
		can "bounce" and cause the LEDs to cycle when you're not pressing the button,
		as suggested here: https://eclass.srv.ualberta.ca/mod/forum/discuss.php?d=851096
		*/
		if (digitalRead(minusButton)== HIGH){
			break; //user released the button, so exit loop
		}
	}
}
// A class could probably have been used, because these functions are so similar,
//however at this point in the course, I don't feel comfortable using c++ classes
void WaitforReleasePlus(){
	while (true){
		delay(10);
		if (digitalRead(plusButton)== HIGH){
			break;
		}
	}
}

void increment(){
	//the main idea is, if LEDi is on, turn it off, check for the next LED,
	//(next iteration of loop) If LED i+1 is also on, turn it off, until you reach an
	//LED that is off. Then, you turn it on and stop. (break)

	for (int i = 0; i < 5; i++){
		//if LEDi is on, turn it off
		if (digitalRead(LEDpins[i]) == HIGH){
			digitalWrite(LEDpins[i],LOW);
			//let the for loop continue to check if the next LED is on. If so, it
			//will repeat. However, if the next LED is off, we will turn it on
			//and break out of the loop in the else case.
		}

		else if (digitalRead(LEDpins[i] == LOW)){
			//could probably have just said else{..} however, I wanted to be
			//a bit more explicit

			//turn LEDi on and break
			digitalWrite(LEDpins[i],HIGH);
			break;
		}

	}
}

//the decrement function looks very similar to increment, so perhaps
//the code could be refactored to use c++ classes, but at the time of doing
//this assignment I do not feel comfortable using them yet, especially
//given that the topic has not been introduced yet, and is a relatively

void decrement(){
	//to count backwards we just need to do the opposite of what would happen
	//if plusButton was pressed, ie if LEDi is off, turn it on, check for the
	//next LED, and if LED i+1 is also off, turn it on until you reach an LED
	//that is already on. Then, we turn it off, and stop. (break)

	for (int i = 0; i < 5; i++){

		if (digitalRead(LEDpins[i]) == LOW){
			digitalWrite(LEDpins[i],HIGH);
			//let the for loop continue to check if the next LED is off. If so, it
			//will repeat. However, if the next LED is on, we will turn it off
			//and break out of the loop in the else case.
		}

		else if (digitalRead(LEDpins[i] == HIGH)){

			//turn LEDi off and break
			digitalWrite(LEDpins[i],LOW);
			break;
		}


	}
}

int main(){
	setup();


	while(true){

		if (digitalRead(plusButton) == LOW){

			increment(); //if user pressed plusButton, increment

			WaitforReleasePlus(); //wait for user to lift finger off button
		}

		else if (digitalRead(minusButton) == LOW){

			decrement(); //elif user pressed minusButton, decrement

			WaitforReleaseMinus(); //wait for user to lift finger off button

		}
	}

	return 0;
}
