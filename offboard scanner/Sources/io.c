#include "io.h"
#include "pwm.h"
#include "keypad.h"
#include "LCD.h"

// predefined strings
const unsigned char maxverti[]   = "vertical(1-5)";
const unsigned char maxhori[]    = "horizontal(1-5)";
const unsigned char validangle[] = "alrite, ";
const unsigned char errorangle[] = "what r u doing?";

const unsigned char resobruh[]   = "resolution(1~9)";
const unsigned char errorres[]   = "invalid, again";
const unsigned char validres[]   = "nice! lets move on";


const unsigned char frequencie[] = "frequency(1~9)";
const unsigned char errorfreq[]  = "error frequency";
const unsigned char validfreq[]  = "good stuff,";

const unsigned char sampling[]   = "samples (1-9)";
const unsigned char errorsamp[]  = "nop bruh";
const unsigned char validsamp[]  = "gotta katchem al";

const unsigned char sensortitle[] = "acl gyh gye mgn";

unsigned char increment;
unsigned char data;
unsigned char frequency;
unsigned char samp_per_ori;
unsigned char max_angle_h;
unsigned char max_angle_t;
unsigned char min_angle_h;
unsigned char min_angle_t;


//************************************main*****************************
void initialization(void){


// get scanning range from user
// max = 90 degree + user defined
// min = 90 - user defined
// max and min is mirrored across 90 degree
  while(1){
    LCD_string(maxverti);

    max_angle_t = getkey();
    LCD_text(max_angle_t);
    if(max_angle_t >= '1' && max_angle_t <= '5'){
      max_angle_t = ((max_angle_t-'0')*5);
      min_angle_t = 90 - max_angle_t;
      max_angle_t += 90;
      break;
    }
    else{
      LCD_string(errorangle);
      onemsdelay(1000);
    }
  }

  while(1){
    LCD_string(maxhori);
    max_angle_h = getkey();
    LCD_text(max_angle_h);
    if(max_angle_h >= '1' && max_angle_h <= '5'){
      max_angle_h = ((max_angle_h-'0')*10);
      min_angle_h = 90 - max_angle_h;
      max_angle_h += 90;
      break;
    }
    else{
      LCD_string(errorangle);
      onemsdelay(1000);
    }
  }

  // check is resolution input is valid,
  // get res btw 1-9
  // get here by completing above loop;
  while(1){
    LCD_string(resobruh);

    increment = getkey();
    LCD_text(increment);
    if(increment >= '1' && increment <= '9'){
      increment = increment-'0';
      break;
    }
    else{
      LCD_string(errorres);
      onemsdelay(1000);
    }
  }

  // frequency loop
  while(1){
    LCD_string(frequencie);

    frequency = getkey();
    LCD_text(frequency);
    if(frequency >= '0' && frequency <= '9'){
      frequency = frequency-'0';
      break;
    }
    else{
      LCD_string(errorfreq);
      onemsdelay(1000);
    }
  }


  // sampling per orientation loop
  // frequency loop
  while(1){
    LCD_string(sampling);

    samp_per_ori = getkey();
    LCD_text(samp_per_ori);
    if(samp_per_ori >= '1' && samp_per_ori <= '9'){
      samp_per_ori = samp_per_ori-'0';
      break;
    }
    else{
      LCD_string(errorsamp);
      onemsdelay(1000);
    }
  }
  LCD_string(sensortitle);
  return;
}
