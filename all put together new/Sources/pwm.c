#include "pwm.h"
#include "laser.h"
#include "io.h"

#define MIN_ANGLE_H 0
#define MAX_ANGLE_H 50
#define MIN_ANGLE_T 0
#define MAX_ANGLE_T 50

  
  int i,j,k;
  float converted;
  int box_or_nobox;
//unsigned  int min_angle_h = 0, max_angle_h = 50, min_angle_t = 0, max_angle_t = 50,laser_measurement = 0;
unsigned  int duty_increment, max_duty_h, max_duty_t, min_duty_h, min_duty_t, step_h, step_t, current_duty_h, current_duty_t;
const char lol[] = "what?"; 

void resetservie(void){

	PWMPRCLK = 0x33; 	// Clock SA&SB = Fbus/8
                   
//	PWMCLK   = 0xA0; 	// Use clockSA and clock SB for pp5 & pp7
                  	/*
                  	1 = Clock SB is the clock source for PWM channel x.
                    0 = Clock B is the clock source for PWM channel x.
                  	*/
	PWMPOL   = 0xF0; 	// high then low for polarity
                  	/*
                  	1 = PWM channel x output is high at the beginning of the period, then goes low when the duty count
                        is reached.
                    0 = PWM channel x output is low at the beginning of the period, then goes high when the duty count
                        is reached.
                    */
	PWMCAE   = 0x00; 	// left aligned for PP5 and PP7
                  	/*
                  	1 = Channel x operates in Center Aligned Output Mode.
                    0 = Channel x operates in Left Aligned Output Mode.
                  	*/
	PWMCTL   = 0xC0; 	// 16-bit PWM channel for pp45 & pp67
                  	/*
                  	1 = Channels 6 and 7 are concatenated to create one 16-bit PWM channel. Channel 6 becomes the
                        high order byte and channel 7 becomes the low order byte. Channel 7 output pin is used as the
                        output for this 16-bit PWM (bit 7 of port PWMP). Channel 7 clock select control-bit
                        determines the clock source, channel 7 polarity bit determines the polarity, channel 7 enable
                        bit enables the output and channel 7 center aligned enable bit determines the output mode.
                    0 = Channels 6 and 7 are separate 8-bit PWMs.
                  	*/
	PWME 	   = 0xA0; 	// enable pp5&pp7
                  	/*
                  	1 = Pulse Width channel x is enabled. The pulse modulated signal becomes available at PWM
                        output bit7 when its clock source begins its next cycle.
                    0 = Pulse Width channel x is disabled.
                    */
	PWMPER67 = 60000; 	// PWM frequency is 50Hz for pp7
                  	/*
                  	Left Aligned Output (CAEx=0)
                    PWMxPeriod=ChannelClockPeriod*PWMPERxCenterAlignedOutput(CAEx=1)
                    PWMx Period = Channel Clock Period * (2 * PWMPERx)
                    */
	PWMDTY67 = 4500; 		// 7.5% duty cycle for middle position horizontal
                  	/*
                  	Polarity = 0 (PPOLx=0)
                    Duty Cycle = [(PWMPERx-PWMDTYx)/PWMPERx] * 100%
                    Polarity = 1 (PPOLx=1)
                    Duty Cycle = [PWMDTYx / PWMPERx] * 100%
                    */
	PWMCNT67 = 0x00; 	// Clear the channel 7
	PWMPER45 = 60000; 	// PWM frequency is 50Hz for pp5
	PWMDTY45 = 4500; 		// Set the duty cycle to 7.5% for middle position vertical
	PWMCNT45 = 0x00; 	// Clear the channel 5
 
}



//cannot input float into serial


void motocontrol(void){


  
  min_duty_h = 2700+(MIN_ANGLE_H/3000)*60000;         // 2700
  max_duty_h = 2700+(MAX_ANGLE_H/3000)*60000;         // 3700
  min_duty_t = 2700+(MIN_ANGLE_T/3000)*60000;         // 2700
  max_duty_t = 2700+(MAX_ANGLE_T/3000)*60000;         // 3700
  duty_increment = (increment/3000)*60000;            // 100
  step_h = (MAX_ANGLE_H-MIN_ANGLE_H)/increment;
  step_t = (MAX_ANGLE_T-MIN_ANGLE_T)/increment;
  
  PWMDTY67 = min_duty_h;
  PWMCNT67 = 0x00; //clear register
  PWMDTY45 = min_duty_t;
  PWMCNT45 = 0x00; //clear register
 
    for (i =0; i < step_h; i++){

//    angle_increased_h = ((max_angle_h-min_angle_h)*j)/increment;
 //   duty_increased_h = (min_angle_h+angle_increased_h)*(1/3000);
 //   current_duty_h = (duty_increased_h+0.045)*1000;
    current_duty_h = min_duty_h+duty_increment*i;
    PWMDTY67 = current_duty_h;
  //  PWMDTY67 = 2700+i*100;
    PWMCNT67 = 0x00; //clear register
    delay(6); //delay function here
     
     for (j =0; j < step_t; j++){ //< or <= steps
     //	while(PWMDTY45<5500){

 //     angle_increased_t = ((max_angle_t-min_angle_t)*j)/increment;
 //     duty_increased_t = (min_angle_t+angle_increased_t)*(1/3000);
 //     current_duty_t = (duty_increased_t+0.045)*1000;
 //     PWMDTY45 = current_duty_t;
       current_duty_t = min_duty_t+duty_increment*j;
       PWMDTY45 = current_duty_t;
        //PWMDTY45 += 100;
       //PWMDTY45 =((min_angle_h+((max_angle_h-min_angle_h)*j)/steps_t)*(1/3000)+0.045)*1000; //set angle
    	PWMCNT45        = 0x00; //clear register
    	delay(6);
    		
//      for (k = 0; k < samp_per_ori; k++){
//        converted = laser_main(); //run laser measurement
//    	  laser_measurement = laser_measurement + converted; //accumulate measurements
//  		}
      
//     	laser_measurement = laser_measurement / samp_per_ori; //average measurements


      
//      box_or_nobox = istherebox(laser_measurement);      // see if its a box

//    	delay(1/frequency); //delay according to required frequency
   	}
  }
}

void delay(int time){
  int i,j;
  for(i = 0; i < time; i++){
    for(j = 0; j < 20000; j++){
    }
  }
}
/*
int istherebox(float Dist){
  
  if(Dist < 0.9 || Dist > 1.2){
    return 0;
  }
  else
    return 1;
}
  */
// need to ignore pole.
// if distance within range and width < 1.5mm
// ignore;