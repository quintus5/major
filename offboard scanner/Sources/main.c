#include "io.h"
#include "globalvar.h"
#include "pwm.h"
#include "laser.h"
#include "sevenseg.h"
#include "keypad.h"
#include "LCD.h"


#include "main_asm.h" /* interface to the assembly module */


//***********************************main************************
void main(void) {

	// configure seven segment display
	sevenseg_config();
	setupkeypad();    // keypad
  LCD_setup();      //lcd display
	// configure laser(timer)
	laser_setup();
	// set all pwm register for servo, and put it in 90 degree hori and vertical
	resetservie();
	EnableInterrupts;
  asm_main(); /* call the assembly function */
	// get all user input here
	initialization();
	// all the scanning is done here	
  motocontrol();
	// may be do calculation here


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
}
