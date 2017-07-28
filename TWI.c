/*
 * TWI.c
 *
 * Created: 7/20/2017 6:55:57 PM
 *  Author: Andreea
 */ 
 #include "TWI.h"
 #include <inttypes.h>
 #include <avr/io.h>

 void TWIInit(void)
 {
	DDRC |= (1<<PC5) | (1<<PC4);
	 //set SCL to 400kHz
	 TWSR = 0x00;
	 TWBR = 0x30;
	 //enable TWI
	 TWCR = (1<<TWEN);
 }


 void TWIStart(void)
 {
	 TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	 while ((TWCR & (1<<TWINT)) == 0);
 }
 //send stop signal
 void TWIStop(void)
 {
	 TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
 }

 void TWIWrite(uint8_t u8data)
 {
	 TWDR = u8data;
	 TWCR = (1<<TWINT)|(1<<TWEN);
	 while ((TWCR & (1<<TWINT)) == 0);
 }

 void TWIWriteACK(uint8_t u8data)
 {
	 TWDR = u8data;
	 TWCR = (1<<TWINT)|(1<<TWEN) | (1<<TWEA);
	 while ((TWCR & (1<<TWINT)) == 0);
 }

 uint8_t TWIReadACK(void)
 {
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	 while ((TWCR & (1<<TWINT)) == 0);
	 return TWDR;
 }
 //read byte with NACK
 uint8_t TWIReadNACK(void)
 {
	 TWCR = (1<<TWINT)|(1<<TWEN);
	 while ((TWCR & (1<<TWINT)) == 0);
	 return TWDR;
 }


 uint8_t TWIGetStatus(void)
{
    uint8_t status;
    //mask status
    status = TWSR & 0xF8;
    return status;
}