#include "io.h"               /* serial io to get all necessary input*/
#include "globalvar.h"      /* contain all global variable*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "main_asm.h" /* interface to the assembly module */



void main(void) {
  /* put your own code here */



	EnableInterrupts;

  asm_main(); /* call the assembly function */
  // this contain all user input variable
  initialization();



  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}