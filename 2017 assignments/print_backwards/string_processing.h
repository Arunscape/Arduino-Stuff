#ifndef _STRING_PROCESSING_H_
#define _STRING_PROCESSING_H_

/* Computes and returns the length of a null-terminated string */
int getStringLength(const char* str);

void testgetStringLength(const char* str);

/* Prints a null-terminated string backwards to the serial port */
void printBackward(const char* str);

#endif
