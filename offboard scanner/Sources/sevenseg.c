#include "sevenseg.h"
#include "globalvar.h"

const char LCD_dot[10] = {0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFE,0x87,0xFF,0xDF}; // if output in meters
const char LCD_nodot[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void onemsdelay(int time){
  int i,j;
  for(i = 0; i <= time; i++){
    for(j = 0; j < 8000; j++){
    }
  }
}

void halfmsdelay(int time){
  int i,j;
  for(i = 0; i <= time; i++){
    for(j = 0; j < 1000; j++){       
    }
  }
}

// configurate seven segment display
void sevenseg_config(void){
	DDRB = 0xFF;
	DDRP = 0xFF;
	DDRJ = 0xFF;
	PTJ = 0b00000010;
}

// print out 4 display
void seveseg_main(int cm){
  int position;
  int c;

  for(c = 0; c <= 10; c++){

		position = cm/100;
    PTP = 0b1110;                  // enable 1st disp
    PORTB = LCD_nodot[position];     // print on disp
    halfmsdelay(frequency);
		position = (cm/10)%10;
		PTP = 0b1101;                  // enable 2nd disp
    PORTB = LCD_nodot[position];   // print on disp
    halfmsdelay(frequency);
		position = cm%10;
		PTP = 0b1011;                  // enable 3rd disp
    PORTB = LCD_nodot[position];   // print on disp
    halfmsdelay(frequency);
  }
  PTP = 0xFF;
}
