#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define set 1
#define clear 0

void printstring(char *msg);
void resetservo(void);
void setupserial(void);
char printcharc(void);
char waitinput(void);