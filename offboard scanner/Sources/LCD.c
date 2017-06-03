#include "LCD.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sevenseg.h"

//Displaying "HELLO" on LCD for Dragon12+ Trainer Board
//with HCS12 Serial Monitor Program installed. This code is for CodeWarrior IDE
//On Dragon12+ LCD data pins of D7-D4 are connected to Pk5-Pk2, En=Pk1,and RS=Pk0,
// Modified from Mazidi's book, Eduardo nebot  28/3/15
// included a loop to write some text
// need to get proper timing and make it more usable
//  CMDWRT4(0x80) is comand for first line first character
// CMDWRT4(0xC0)   is comand for second line first character
// need to evaluate proper delay

void LCD_setup(void){

  DDRK = 0xFF;
  CMDWRT4(0x33);   //reset sequence provided by data sheet
  delayforlcd(1);
  CMDWRT4(0x32);   //reset sequence provided by data sheet
  delayforlcd(1);
  CMDWRT4(0x28);   //Function set to four bit data length
                                   //2 line, 5 x 7 dot format
  delayforlcd(1);
  CMDWRT4(0x06);  //entry mode set, increment, no shift
  delayforlcd(1);
  CMDWRT4(0x0E);  //Display set, disp on, cursor on, blink off
}

// do the printing on LCD
void LCD_string(unsigned char *MSG1){
  int i=0;
//  delayforlcd(1);
  CMDWRT4(0x01);  //Clear display
  delayforlcd(1);
  CMDWRT4(0x80);  //set start posistion, home position
//  delayforlcd(1);

  while (MSG1[i] != 0x00) {   // print sentence
    DATWRT4(MSG1[i++]);
  }
}

void LCD_text(unsigned char keypressed){

  CMDWRT4(0xC0);  //set start posistion, 2nd row left most
  delayforlcd(1);
  DATWRT4(keypressed);
  delayforlcd(250);
}

void LCD_sensorval(unsigned char *value_string){
     int i=0; 
        while (value_string[i] != 0x00) {    
        DATWRT4(value_string[i++]);
     }
}

void CMDWRT4(unsigned char command)
  {
        unsigned char x;

        x = (command & 0xF0) >> 2;         //shift high nibble to center of byte for Pk5-Pk2
        LCD_DATA =LCD_DATA & ~0x3C;       //clear bits Pk5-Pk2
        LCD_DATA = LCD_DATA | x;         //sends high nibble to PORTK
        LCD_CTRL = LCD_CTRL & ~RS;      //set RS to command (RS=0)
        LCD_CTRL = LCD_CTRL | EN;      //rais enable
        delayforlcd(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //Drop enable to capture command
        delayforlcd(15);                  //wait
        
        x = (command & 0x0F)<< 2;          // shift low nibble to center of byte for Pk5-Pk2
        LCD_DATA =LCD_DATA & ~0x3C;       //clear bits Pk5-Pk2
        LCD_DATA =LCD_DATA | x;          //send low nibble to PORTK
        LCD_CTRL = LCD_CTRL | EN;       //rais enable
        delayforlcd(5);
        LCD_CTRL = LCD_CTRL & ~EN;    //drop enable to capture command
        delayforlcd(15);
  }


void DATWRT4(unsigned char data){
    unsigned char x;

        x = (data & 0xF0) >> 2;
        LCD_DATA =LCD_DATA & ~0x3C;
        LCD_DATA = LCD_DATA | x;          // output upper 4 bits 
        LCD_CTRL = LCD_CTRL | RS;         // select data register
        LCD_CTRL = LCD_CTRL | EN;         // pull e high
 //       delayforlcd(1);
        asm("nop"); // dummy statements to lengthen E
        asm("nop"); // "
        asm("nop");
        LCD_CTRL = LCD_CTRL & ~EN;        // pull e low
//        delayforlcd(5);

        x = (data & 0x0F)<< 2;
        LCD_DATA =LCD_DATA & ~0x3C;
        LCD_DATA = LCD_DATA | x;
        LCD_CTRL = LCD_CTRL | EN;
  //      delayforlcd(1);
        asm("nop"); // dummy statements to lengthen E
        asm("nop"); // "
        asm("nop");
        LCD_CTRL = LCD_CTRL & ~EN;
        halfmsdelay(5);
}
