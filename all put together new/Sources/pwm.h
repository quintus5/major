#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "globalvar.h"

#ifndef pwm_h
#define pwm_h

void resetservie(void);
void motocontrol(void);
void delay(int);
int istherebox(float);

#endif