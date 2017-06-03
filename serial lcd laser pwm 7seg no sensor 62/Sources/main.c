#include "io.h"
#include "globalvar.h"
#include "pwm.h"
#include "laser.h"
#include "sevenseg.h"


#include "main_asm.h" /* interface to the assembly module */


//***********************************main************************
void main(void) {
  
	// configure seven segment display
	sevenseg_config();
	// configure laser(timer)
	laser_setup();
	EnableInterrupts;
	// set all pwm register for servo, and put it in 90 degree hori and vertical
	resetservie();
  asm_main(); /* call the assembly function */
	// get all user input here
	initialization();
  DisableInterrupts;
	
	// all the scanning is done here
  motocontrol();
	


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
}
