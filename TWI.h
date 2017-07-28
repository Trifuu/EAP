/*
 * TWI.h
 *
 * Created: 7/21/2017 3:01:39 PM
 *  Author: mariu
 */ 
/*
 * TWI.h
 *
 * Created: 7/20/2017 6:55:39 PM
 *  Author: Andreea
 */ 

#include <inttypes.h>

#ifndef TWI_H_
#define TWI_H_



void TWIInit(void);
void TWIStart(void);
void TWIStop(void);
void TWIWrite(uint8_t);
uint8_t TWIReadACK(void);
uint8_t TWIReadNACK(void);
uint8_t TWIGetStatus(void);

#endif /* TWI_H_ */