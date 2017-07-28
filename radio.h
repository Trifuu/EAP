
/*
 * radio.h
 *
 * Created: 7/23/2017 3:26:21 PM
 *  Author: mariu
 */ 
#include "lcd.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "TWI.h"

uint8_t vol2;

 void radio_write_reg(uint8_t reg, uint8_t high, uint8_t low);
 volatile uint8_t * radio_read();
 void radio_freq(uint8_t freq);
 void start_write();
 void volum(uint8_t vol);
 void mute();
 void unmute();
 void select_radio(int nr);
 void init_radio();
 void uart_init();
 unsigned char uart_receive ();
 void comenzi_wifi();
  void init_shift();
   void shift_out(uint8_t shift);
   uint8_t set_volum(int a);