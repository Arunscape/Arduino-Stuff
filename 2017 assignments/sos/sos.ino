// Arun Woosaree
/* Some of the following code is copied from http://ugweb.cs.ualberta.ca/~c274/web/ArduinoIntro/code/Lab_SOS/SOS07/SOS07.ino
which the assignment asks to improve on and complete. Since we are told to not
use the pushbutton, code and comments related to the pushbutton have
been removed.

Commented out pieces of code beginning with Serial.begin or Serial.print were used for debugging purposes and can be re-enabled
for more verbose output in the serial monitor. "DOT " represents a dot, "DASH " represents a dash, and each "X" represents
a length of time, the same as dotTime, for when the LED is off
*/

// the external LED is attached to this pin.
// onboard LED is attached to pin 13,
int ledPin = 13;

// slider of potentiometer attached to this analog input
int analogInPin = 0;

// don't need to set dotTime because the value is read from the potentiometer in
// the main loop, but we need to declare it as a global variable so all the
// functions that use it can see it
int dotTime ;

//the potentiometer is connected to analog pin 0
int potentiometer = 0;

// we take the code for sending dot and dash and put them
// into two methods sendDot and sendDash

//function to send a dot
void sendDot() {
  // a dot is one unit long
  digitalWrite(ledPin, HIGH);
  delay(dotTime);

  //Serial.print("DOT "); //debugging

  //insert one dot gap
  digitalWrite(ledPin, LOW);
  delay(dotTime);

  //Serial.print("X "); //debugging
}

//function to send a dash
void sendDash() {
  // a dash is 3 dots long
  digitalWrite(ledPin, HIGH);
  delay(3 * dotTime);

  //Serial.print("DASH ");  //debugging

  // an inter-element gap is one dot
  digitalWrite(ledPin, LOW);
  delay(dotTime);

  //Serial.print("X "); //debugging
}

// code for sending the gaps
// send short gap
void sendShortGap() {
  // we assume that we are preceeded by an inter-element gap.
  // so that we have 3 dots of gap
  delay(2 * dotTime);

  //Serial.print("XX ");  //debugging
}

//medium gap
void sendMediumGap() {
// with the way I set up the sendS() and sendO() functions, there is already a
// 3 dot length gap. A medium gap should last 7 dots, so we need 4 more dots of delay
  delay(4 * dotTime);

  //Serial.print("XXXX\n");  //debugging
}

// morse code for the letter S
void sendS(){
  //do the letter S
  sendDot();
  sendDot();
  sendDot();
  //short gap
  sendShortGap();
}

// morse code for the letter O
void sendO(){
  //do the letter 0
  sendDash();
  sendDash();
  sendDash();
  //short gap
  sendShortGap();
}

void setup() {
  // configure ledPin to be a digital output
  //Serial.begin(9600);  //debugging
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // need to read analog input from the potentiometer so we can vary the blink
  // speed. This is done by adjusting dotTime
  dotTime = analogRead(potentiometer) ;

  //send the message
  sendS();
  sendO();
  sendS();
  //now that the word is done, we need to add a medium gap for between words
  sendMediumGap();
}
