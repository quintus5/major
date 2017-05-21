#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define set 1
#define clear 0

void printstring(char *msg);
void setupserial(void);
char printcharc(void);

// predefined strings
const char wannareset[] = "do you wanna reset servo? (y/n)\r\n";
const char resmsg[]     = "\r\ninput resolution (1~9): \r\n";
const char errorres[]   = "\r\ninvalid resolution enter again\r\n";
const char errorinput[] = "\r\ni cannot understand, please try again\r\n";
const char msg[]        = "\r\nresetting servo...\r\n";
const char validinput[] = "\r\ninput is valid, proceed to scanning...\r\n";


// setup serial port
void setupserial(){

  SCI0BDH = 0;
  SCI0BDL = 156;    // baurate of 9600
  SCI0CR1 = 0x00;   //  control register with enable bit on
  SCI0CR2 = 0b00101100;   // enable RIE TIE TE RE
  DDRB = 0xFF;
  PORTB = clear;
}


interrupt 20 void SCI0_ISR(void){     // row 20 of interrupt vector table, (SCI0)
 
    char garbage = SCI0DRL;           // reading data register will clear the flag
    SCI0SR1 &= ~SCI0SR1_RDRF_MASK;    // resets the flag for next time       
    PORTB = set;                      // get ready to print once data received
} 


void resetservo(){
  // call pwm function to reset servo to initial top left position
  // terminate all current task
  printstring(msg); 
}


// output NULL terminated string using polling method
void printstring(char *string){
  
  while((*string) != '\0'){
      while(!(SCI0SR1&SCI0SR1_TDRE_MASK));
      SCI0DRL = *string;
      string++; 
  }
}

// get data from serial and print on screen, polling, return data
char printcharc(void){
  char data = SCI0DRL;
  while(!(SCI0SR1&SCI0SR1_TDRE_MASK));
  SCI0DRL = data;
  return data;
}


void main(){

  int increment;
  char data;
  
  setupserial();
  _asm CLI;

// initial setup
// loop until everything is acquired
// may need to change loop condition
while(1){
    printstring(wannareset);
    
    // wait till port b is set(data is given)
    // store data then get out of loop
    while(PORTB == clear){
       if(PORTB == set){      
         data = printcharc();      
         PORTB = clear;
         break;
       }
    }
    
    if(data == 'y'){                   // check if wanna resetservo
        resetservo();                  
    }
    
    else if(data == 'n'){               // if not then poll for increment
      printstring(resmsg);
    
      // wait till somthing is given
      while(PORTB == clear){
         if(PORTB == set){    
         increment = printcharc();      
         PORTB = clear;
         break;
       }
    }
      
      if(increment >= '1' && increment <= '9'){    // if not gonna resetservo, then adjust increment
        increment = increment-'0';                 // get the (int)data
        printstring(validinput);
        break;                                     // get out of loop when everything required is available
      }
      else{
        printstring(errorres);             // print error message if outside range
      }
    }
    else{
       printstring(errorinput);             // print error message if typed other than n or y
    }
    
} 
  while(1);
}
