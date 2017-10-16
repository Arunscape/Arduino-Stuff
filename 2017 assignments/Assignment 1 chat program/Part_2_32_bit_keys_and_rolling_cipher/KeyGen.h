#ifndef _KEYGEN_H_
#define _KEYGEN_H_

// header file for some higher-level functions to read input from the serial monitor

//generate private key
uint32_t privKeyGen() ;

uint32_t mulMod( uint32_t a, uint32_t b, uint32_t m );

uint32_t powModFast(uint32_t a, uint32_t b, uint32_t m);

uint32_t next_key(uint32_t current_key);

#endif
