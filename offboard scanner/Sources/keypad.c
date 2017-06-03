#include "keypad.h"
#include "sevenseg.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

const unsigned char keypad[4][4] =
{
'1','2','3','A',
'4','5','6','B',
'7','8','9','C',
'*','0','#','D'
};

unsigned char column,row;

// setup 4x4 key pad
void setupkeypad(void){

  DDRP |=0x0F;                           // 7seg as OUTPUT
  PTP |=0x0F;                            //TURN OFF 7SEG LED
  DDRA = 0x0F;                           //MAKE ROWS INPUT AND COLUMNS OUTPUT
  DDRB = 0xFF;
  DDRJ = 0xFF;
  PTJ = 0;
}

char getkey(void){

   char key;
   while(1){
      do{                                 //OPEN do1
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }
      while(row == 0x00);                //WAIT UNTIL KEY PRESSED //CLOSE do1

      do{                                 //OPEN do2
         do{                              //OPEN do3
            halfmsdelay(1);                   //WAIT
            row = PORTA & 0xF0;           //READ ROWS
         }
         while(row == 0x00);             //CHECK FOR KEY PRESS //CLOSE do3

         halfmsdelay(15);                     //WAIT FOR DEBOUNCE
         row = PORTA & 0xF0;
      }
      while(row == 0x00);                //FALSE KEY PRESS //CLOSE do2

      while(1){                           //OPEN while(1)
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x01;                   //COLUMN 0 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 0
            column = 0;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x02;                   //COLUMN 1 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 1
            column = 1;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x04;                   //COLUMN 2 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 2
            column = 2;
            break;                        //BREAK OUT OF while(1)
         }
         PORTA &= 0xF0;                   //CLEAR COLUMN
         PORTA |= 0x08;                   //COLUMN 3 SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
         if(row != 0x00){                 //KEY IS IN COLUMN 3
            column = 3;
            break;                        //BREAK OUT OF while(1)
         }
         row = 0;                         //KEY NOT FOUND
      break;                              //step out of while(1) loop to not get stuck
      }                                   //end while(1)


     if(row == 0x10){
         key = keypad[0][column];         // return the key pressed
      }
      else if(row == 0x20){
        key = keypad[1][column];
      }
      else if(row == 0x40){
         key = keypad[2][column];
      }
      else if(row == 0x80){
         key = keypad[3][column];
      }

      do{
         halfmsdelay(15);
         PORTA = PORTA | 0x0F;            //COLUMNS SET HIGH
         row = PORTA & 0xF0;              //READ ROWS
      }while(row != 0x00);                //MAKE SURE BUTTON IS NOT STILL HELD
    return key;
   }                                      //CLOSE WHILE(1)

}                                         //CLOSE MAIN
