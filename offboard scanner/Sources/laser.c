#include "laser.h"
#include "sevenseg.h"

  int Fraw;
	int Rraw;
	int Pulsewidth;
	int Distance_in_cm;

//laser setup
void laser_setup(void){
	DDRT  = 0xFD; 			//set PT1 as input
	TSCR1 = 0x80;	      //enable timers
  /*
  TSCR1_TFFCA:    4    ; Timer Fast Flag Clear All
  TSCR1_TSFRZ:    5    ; Timer and Modulus Counter Stop While in Freeze Mode
  TSCR1_TSWAI:    6    ; Timer Module Stops While in Wait
  TSCR1_TEN:      7    ; Timer Enable
  */
	TSCR2 = 0x04; 			//no overflow, prescaler 16 to ensure a maximum 40m measurement
	/*
	TSCR2_PR0:      0    ; Timer Prescaler Select Bit 0
  TSCR2_PR1:      1    ; Timer Prescaler Select Bit 1
  TSCR2_PR2:      2    ; Timer Prescaler Select Bit 2
  TSCR2_TCRE:     3    ; Timer Counter Reset Enable
  TSCR2_TOI:      7    ; Timer Overflow Interrupt Enable
  */
	TIOS  = 0xFD; 			//set PT1 as input capture
	/*
	0 The corresponding channel acts as an input capture.
  1 The corresponding channel acts as an output compare.
	TIOS_IOS0:      0    ; Input Capture or Output Compare Channel Configuration Bit 0
  TIOS_IOS1:      1    ; Input Capture or Output Compare Channel Configuration Bit 1
  TIOS_IOS2:      2    ; Input Capture or Output Compare Channel Configuration Bit 2
  TIOS_IOS3:      3    ; Input Capture or Output Compare Channel Configuration Bit 3
  TIOS_IOS4:      4    ; Input Capture or Output Compare Channel Configuration Bit 4
  TIOS_IOS5:      5    ; Input Capture or Output Compare Channel Configuration Bit 5
  TIOS_IOS6:      6    ; Input Capture or Output Compare Channel Configuration Bit 6
  TIOS_IOS7:      7    ; Input Capture or Output Compare Channel Configuration Bit 7
  */
}

//laser main
int laser_main(void){



	TCTL4 = 0x04; 	//set PT1 to capture rising edge
	/*
	TCTL4_EDG0A:    0    ; Input Capture Edge Control 0A
	TCTL4_EDG0B:    1    ; Input Capture Edge Control 0B
	TCTL4_EDG1A:    2    ; Input Capture Edge Control 1A
	TCTL4_EDG1B:    3    ; Input Capture Edge Control 1B
	TCTL4_EDG2A:    4    ; Input Capture Edge Control 2A
	TCTL4_EDG2B:    5    ; Input Capture Edge Control 2B
	TCTL4_EDG3A:    6    ; Input Capture Edge Control 3A
	TCTL4_EDG3B:    7    ; Input Capture Edge Control 3B
	*/
	TFLG1 = 0x02; 	//clear C1F+

	while (!(TFLG1 & TFLG1_C1F_MASK)); 	//detect if rising edge occurs
	Rraw  = TC1;  	//record if occurred
	TCTL4 = 0x08; 	//set PT1 to capture falling edge
	TFLG1 = 0x02; 	//clear C1F

	while (!(TFLG1 & TFLG1_C1F_MASK)); 	//detect if falling edge occurs
	Fraw  = TC1;  	//then record
	Pulsewidth = (Fraw - Rraw);
	Distance_in_cm  = (Pulsewidth/15); 	//display in cm
	return Distance_in_cm;
}
