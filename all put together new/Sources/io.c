#include "io.h"
#include "pwm.h"
#include "globalvar.h"



// predefined strings
const unsigned char wannareset[] = "do you wanna reset servo? (y/n)\r\n";
const unsigned char resetservy[] = "\nresetting servo...\r\n";
const unsigned char errorinput[] = "\ni cannot understand, please try again\r\n";
const unsigned char noreset[]    = "\nso u dont want reset, interesting...\r\n";

const unsigned char resobruh[]    = "\ninput resolution (1~9): \r\n";
const unsigned char errorres[]   = "\ninvalid resolution enter again\r\n";
const unsigned char validres[]   = "\nvalid resolution, lets move on...\r\n";


const unsigned char frequencie[]   = "\nsup bruh, gimme some frequency (1~9):\r\n";
const unsigned char errorfreq[]  = "\nerror frequency...\r\n";
const unsigned char validfreq[]  = "\nfrequency is valid, you are good...\r\n";

const unsigned char sampling[]   = "\nwhats your sample per orientation (1~9):\r\n";
const unsigned char errorsamp[]  = "\nerror sampling, you can do better\r\n";
const unsigned char validsamp[]  = "\nsamp is valid, proceed to scanning...\r\n";


unsigned char increment;
unsigned char data;
unsigned char frequency;
unsigned char samp_per_ori;


// setup serial port
void setupserial(){

  SCI1BDH = 0;
  SCI1BDL = 156;    // baurate of 9600
  SCI1CR1 = 0x00;   //  control register with enable bit on
  SCI1CR2 = 0b00101100;   // enable RIE TE RE
  DDRA = 0xFF;
  PORTA = clear;
}


interrupt 21 void SCI1_ISR(void){     // row 20 of interrupt vector table, (SCI0)
 
    char garbage = SCI1DRL;           // reading data register will clear the flag
    SCI1SR1 &= ~SCI0SR1_RDRF_MASK;    // resets the flag for next time       
    PORTA = set;                      // get ready to print once data received
} 



void resetservo(){
  // call pwm function to reset servo to initial top left position
  // terminate all current task
  printstring(resetservy);
  resetservie(); 
}


// output NULL terminated string using polling method
void printstring(char *string){
  
  while((*string) != '\0'){
      while(!(SCI1SR1&SCI1SR1_TDRE_MASK));
      SCI1DRL = *string;
      string++; 
  }
  PORTA = clear;
}


// get data from serial and print on screen, polling, return data
char printcharc(void){
  char data = SCI1DRL;
  while(!(SCI1SR1&SCI1SR1_TDRE_MASK));
  SCI1DRL = data;
  return data;
}


// get data from serial port
char waitinput(void){
  char data;
  while(PORTA == clear);
  if(PORTA == set){     
    data = printcharc();      
    PORTA = clear;
    return data;
  }
}



//************************************main*****************************
void initialization(void){

  DisableInterrupts;
  setupserial();
  _asm CLI;


  // check if user wannt reset servo to original position
  while(1){
    printstring(wannareset);
    
    data = waitinput();
    if(data == 'y'){
      resetservo();
    } 
    else if(data == 'n'){
      printstring(noreset);
      break;
    } 
    else{
      printstring(errorinput);
    } 
  }


  // check is resolution input is valid,
  // get res btw 1-9
  // get here by completing above loop;
  while(1){
    printstring(resobruh);
    
    increment = waitinput();
    if(increment >= '1' && increment <= '9'){    
      increment = increment-'0';               
      printstring(validres);
      break;
    }
    else{
      printstring(errorres);
    }
    
  }


  // frequency loop
  while(1){
    printstring(frequencie);
    
    frequency = waitinput();
    if(frequency >= '1' && frequency <= '9'){    
      frequency = frequency-'0';               
      printstring(validfreq);
      break;
    }
    else{
      printstring(errorfreq);
    }
  }


  // sampling per orientation loop
  // frequency loop
  while(1){
    printstring(sampling);
    
    samp_per_ori = waitinput();
    if(samp_per_ori >= '1' && samp_per_ori <= '9'){    
      samp_per_ori = samp_per_ori-'0';               
      printstring(validsamp);
      break;
    }
    else{
      printstring(errorsamp);
    }
  }
  return;
}
