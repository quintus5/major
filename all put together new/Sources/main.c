#include "io.h"
#include "globalvar.h"
#include "pwm.h"
#include "laser.h"
#include "sevenseg.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "main_asm.h" /* interface to the assembly module */


//***********************************main************************
void main(void) {


//	LCD_config();
	EnableInterrupts;

  asm_main(); /* call the assembly function */
  initialization();
  motocontrol();



  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
}
