#define F_CPU 16000000UL

#include "radio.h"


int main(void)
{
    lcd_init(LCD_DISP_ON_CURSOR);
	init_shift();
    init_radio();
	shift_out(0b00001111);
	uart_init();
	
	
	
	
	sei();
	while(1){
		comenzi_wifi();
		
	}
}