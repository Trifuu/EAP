 #define F_CPU 16000000UL
 #include "lcd.h"

 #include <avr/io.h>
 #include "TWI.h"
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include "radio.h"
 int verif=0;
 
 void radio_write_reg(uint8_t reg, uint8_t high, uint8_t low){
	 TWIStart();
	 TWIWrite(0x22);
	 TWIWrite(reg);
	 TWIWrite(high);
	 TWIWrite(low);
	 TWIStop();
 }
 volatile uint8_t * radio_read(){
	 volatile static uint8_t data[26];
	 TWIStart();
	 TWIWrite(0x22);
	 TWIWrite(2);
	 TWIStop();
	 
	 TWIStart();
	 TWIWrite(0x23);
	 for (int i=0;i<12;i++)
		data[i]=TWIReadACK();
	 data[12]=TWIReadACK();
	 TWIStop();
	 return data;
 }
 void radio_freq(uint8_t freq){
	 char text[16];
	 
	 freq=freq-870;
	 uint8_t freqH=freq>>2;
	 uint8_t freqL=(freq & 3) <<6;
	 int rez=freq;
	 sprintf(text,"%d.%d MHz",(870+rez)/10,(870+rez)%10);
	 lcd_clrscr();
	 lcd_home();
	 lcd_puts(text);
	 lcd_gotoxy(0,1);

	 radio_write_reg(3,freqH,freqL+0x10);
 }
 void start_write(){
	 TWIInit();
	 radio_write_reg(2,0xc0,0x03);
	 radio_write_reg(2,0xd0,0x0d);
 }
 //vol ia valori de la 0-15
 void volum(uint8_t vol){
	 if(vol>15)
	 vol=15;
	 radio_write_reg(5,0x88,vol);
 }
 void mute(){
	 radio_write_reg(2,0x00,0x0d);
 }
 void unmute(){
	 radio_write_reg(2,0xd0,0x0d);
 }
 void select_radio(int nr){
	 switch(nr){
		case 1:  radio_freq(890); lcd_puts("Radio ZU"); break;
		case 2:  radio_freq(908); lcd_puts("Magic FM"); break;
		case 3:  radio_freq(983); lcd_puts("Radio Romania"); break;
		case 4:  radio_freq(1053); lcd_puts("R. Actualitati"); break;
		case 5:  radio_freq(961); lcd_puts("Kiss FM"); break;
		case 6:  radio_freq(1067); lcd_puts("Radio Europa FM"); break;
		case 7:  radio_freq(1019); lcd_puts("Romantic FM"); break;
		case 8:  radio_freq(1006); lcd_puts("Rock FM"); break;
		case 9:  radio_freq(1048); lcd_puts("Radio Muzical"); break;
		case 10: radio_freq(1034); lcd_puts("Radio 7"); break;
		case 11: vol2 =vol2-1; volum(vol2);shift_out(set_volum(vol2)); break;
		case 12: vol2 =vol2+1; volum(vol2);shift_out(set_volum(vol2)); break;
	 }
 }
 uint8_t set_volum(int a){
	 switch(a-1){
		case 2: 
		case 1: return 0b00000001;
		case 3: 
		case 4: return 0b00000011;
		case 5:
		case 6: return 0b00000111;
		case 7:
		case 8: return 0b00001111;
		case 9:
		case 10: return 0b00011111;
		case 11:
		case 12: return 0b00111111;
		case 13:
		case 14: return 0b01111111;
		case 15:
		case 16: return 0b11111111;
		
	 }
 }
 void init_radio(){
	 start_write();
	 vol2=0b00001000;
	 volum(vol2);
	 
	 radio_freq(908);
	 lcd_puts("Magic FM");
	 //mute();
 }
 void uart_init(){
	 PCICR |=(1<<PCIE1);	 PCMSK1 |=(1<<PCINT9);
	 UBRR0 = 8;
	 UCSR0B |= (1<<RXEN0);
	 
 }
 unsigned char uart_receive (){
	 while (!(UCSR0A & (1<<RXC0)));
	 return UDR0;
 }
 void comenzi_wifi(){
	 static int i,ok;
	 static char str[6];
	 unsigned char c;
	 c = uart_receive();
	 if(c!='$'){
		 str[i]=c;
		 i++;
		 ok=0;
	 }
	 else{
		 if(ok==0){
			 ok=1;
			 i=0;
			 if(strlen(str)<3){
				 int nr;
				 nr=atoi(str);
				 select_radio(nr);
			 }
			 memset(str,"\0",5);
		 }
	 }
 }
 ISR(PCINT1_vect){
	 cli();
	 if(PINC & (1<<PINC1)){
		 if( verif == 0 ){
			 unmute();
			 verif=1;
		 }else
		 if( verif == 1 ){
			 mute();
			 verif=0;
		 }
	 }
	 sei();
 }
 
 void init_shift(){
	 DDRC |= (1<<PC0) | (1<<PC2) | (1<<PC3);
 }
 
 void shift_out(uint8_t shift){
	 uint8_t i;
	 for(i=0;i<8;i++){
		PORTC &= ~(1<<PC0);
		PORTC &= ~(1<<PC2);
		
		PORTC |= ((1 & shift)<<PC0);
		shift>>=1;
		
		_delay_ms(1);
		PORTC |= (1<<PC2);
		_delay_ms(1);
		
	 }
	 
	 
	 PORTC |= (1<<PC3);
	 _delay_ms(1);
	 PORTC &= ~(1<<PC3);
 }
 