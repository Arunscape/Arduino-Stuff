# Arduino-Stuff
A collection of Arduino projects I have worked on at the University of Alberta

## File structure:

### 2017 Assignments
* [Assignment 1 Chat Program](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/Assignment%201%20chat%20program/Part_2_32_bit_keys_and_rolling_cipher)

  This was the first major Arduino assignment, which allows for serial communication between 2 Arduino Megas
  Communication is done through the serial monitor, and the chat is encrypted using a Diffie-Hellman key-exchange and
  XOR encryption
  
* [Assignment 2 Restaurant Finder](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/Assignment%202%20restaurant%20finder/part_2)

  Using the Arduino, a Display, and a joystick we made an app that lets you search for restaurants in a map of Edmonton
Restaurants are sorted by distance, and you can choose the minimum rating to display
 
* [Final Project](https://github.com/bojotamara/arduino-chess-game)
 
  A 2-player chess game for the Arduino, complete with some lesser-known special moves like the En Passant! Exists in an external repo [here](https://github.com/bojotamara/arduino-chess-game)
 
 #### Weekly Exercises
 
* [IPV4 Octets](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/IPV4_Octets)
 
  This program accepts input for a 32 bit unsigned integer that represents an ip
address in decimal form. The program will then print the value of each octet in
the ip address you entered.
  
* [Stack](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/Stack(desktop))
  
  An implementation of the Stack Abstract Data Type (ADT) in C++ using linked lists
   
* [Counting Lights](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/countinglights)
   
  A 5 bit binary counter using LEDs
    
* [Faster Reading from SD](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/faster_reading_from_sd)
    
  A part of the [Restaurant Finder](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/Assignment%202%20restaurant%20finder/part_2) assignment, where we read data from the SD card mor efficiently. If the item requested is already in a loaded block in memory, the Arduino won't read the same block again, it will just take the value loaded in memory
     
* [GCD Recursion](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/gcd_recursion_(desktop))
    
  A recursive implementation to find the greatest common divisor of two integers, using Euclid's algorithm
      
* [SOS](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/sos)
    
  Send SOS signal with Morse Code using an LED and variable blink speed
  
 * [String Processing](https://github.com/Arunscape/Arduino-Stuff/tree/master/2017%20assignments/print_backwards)
      
  Custom string processing, including a print backwards function, and custom function to get sting length
