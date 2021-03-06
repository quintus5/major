#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "globalvar.h"

#ifndef io_h
#define io_h

void printstring(char *msg);
void resetservo(void);
void setupserial(void);
char printcharc(void);
char waitinput(void);
void initialization(void);

#endif