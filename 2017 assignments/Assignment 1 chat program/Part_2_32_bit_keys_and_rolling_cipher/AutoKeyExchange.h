#ifndef _AUTOKEYEXCHANGE_H_
#define _AUTOKEYEXCHANGE_H_



bool wait_on_serial3( uint8_t nbytes, long timeout );

/** Writes an uint32_t to Serial3, starting from the least-significant
 * and finishing with the most significant byte.
 */
void uint32_to_serial3(uint32_t num);

/** Reads an uint32_t from Serial3, starting from the least-significant
 * and finishing with the most significant byte.
 */
uint32_t uint32_from_serial3();

//the functions above were given on eclass, the below function was made by us

//commence the auto key exchange to get the partnerPubKey
uint32_t AutoKeyExchange(uint32_t pubkey);
#endif
