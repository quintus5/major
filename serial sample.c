#include <stdio.h>


#define set 1
#define clear 0

// setup serial port
void setupserial(){
  printf("setting serial...\n");
  return;
}

// reset servo
void resetservo(){
  // call pwm function to reset servo to initial top left position
  // terminate all current task
  printf("resetting servo...\n");
  return;
}


// output NULL terminated string using polling method
void printstring(char *msg){
  while(*msg != '\0'){
    printf("%c",*msg);
    msg++;
  }
  return;
}

// get a character by polling
char getcharc(void){
  char data;
  scanf("%s", &data);
  return data;
}

int main() {

  int increment;
  char data;

  char *wannareset = "do you wanna reset servo? (y/n)";
  char *resmsg = "input resolution (1~9): ";
  char *error = "invalid resolution enter again";

  setupserial();


// initial setup
// loop until everything is acquired
// may need to change loop condition
  while(1){

    printstring(wannareset);
    data = getcharc();

    if(data == 'y'){                   // check if wanna resetservo
        resetservo();
    }
    else if(data == 'n'){
      printstring(resmsg);
      increment = getcharc();

      if(increment >= '1' && increment <= '9'){    // if not gonna resetservo, then adjuest increment
        increment = increment-'0';                 // get the (int)data
        printf("%i\n", increment);
        break;                                     // get out of loop when everything required is available
      }
      else{
        printstring(error);             // print error message if not what we wanted
      }
    }
  }
  return 0;
}
