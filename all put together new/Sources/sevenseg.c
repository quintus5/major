#include "sevenseg.h"

const char LCD_dot[10] = {0xBF,0x86,0xDB,0xCF,0xE6,0xED}; // if output in meters
const char LCD_nodot[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void delay(void){
	int m;
	for(m = 0; m < 30000; m++);
}

// configurate seven segment display
void sevenseg_config(void){
	DDRB = 0xFF;
	DDRP = 0xFF;
	DDRJ = 0xFF;
	PTJ = 0b00000010;
}

// print out 4 display
void seveseg_main(int mm){
  int position;
  int c;
  for(c = 0; c < 10; c++){

		position = mm/1000;
    PTP = 0b1110;                  // enable 1st disp
    PORTB = LCD_dot[position];     // print on disp
    delay();
		position = (mm/100)%10;
		PTP = 0b1101;                  // enable 2nd disp
    PORTB = LCD_nodot[position];   // print on disp
    delay();
		position = (mm/10)%10;
		PTP = 0b1011;                  // enable 3rd disp
    PORTB = LCD_nodot[position];   // print on disp
    delay();
		position = mm%10
		PTP = 0b0111;                  // enable 4th disp
    PORTB = LCD_nodot[position];   // print on disp
    delay();
  }
	PTP = clear;
}
