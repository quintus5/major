#include "pwm.h"
#include "laser.h"
#include "sevenseg.h"
#include "globalvar.h"
#include "sensor_on_lcd.h"


int i, j, k, h, h_final, w, w_final;
int converted;
int box_or_nobox; // output 1 or 0 on whether a object is found
unsigned int laser_measurement = 0, central;
unsigned int duty_increment, max_duty_h, max_duty_t, min_duty_h, min_duty_t, step_h, step_t, current_duty_h, current_duty_t, h_angle, w_angle, width, height, area;

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

void motocontrol(void){
																		// max range |
  min_duty_h = 2700+min_angle_h*20;         // 3500
  max_duty_h = 2700+max_angle_h*20;         // 5500
  min_duty_t = 2700+min_angle_t*20;         // 4000
  max_duty_t = 2700+max_angle_t*20;         // 5000
  duty_increment = increment*20;            // increment is 5 then 100
  step_h = (max_angle_h-min_angle_h)/increment;				//
  step_t = (max_angle_t-min_angle_t)/increment;

  PWMDTY67 = min_duty_h;
  PWMCNT67 = 0x00; //clear register
  PWMDTY45 = min_duty_t;
  PWMCNT45 = 0x00; //clear register

  for (i = 0; i <= step_h; i++){
			// increment current duty cycle by input resolution
 
//maximum horizontal distance is 1.43 m for 1.2 m measurement
    	current_duty_h = min_duty_h+duty_increment*i;
    	PWMDTY67 = current_duty_h;	// output to PWM
    	PWMCNT67 = 0x00; //clear register
//    	halfmsdelay(frequency); //delay function here

    	for (j = 0; j <= step_t; j++){ 			//< or <= steps
      	 //maximum vertical distance is 2.06 m for 1.2 m measurement
        current_duty_t = min_duty_t+duty_increment*j;
    		PWMDTY45 = current_duty_t;
    		PWMCNT45 = 0x00; //clear register
 //   		halfmsdelay(frequency*10);				// delay to wait laser and sensor

				for (k = 0; k <= samp_per_ori; k++){
					// get distance data from laser
					// take multiple scan up to user		
					converted = laser_main(); 													//run laser measurement
					laser_measurement = laser_measurement + converted;  //accumulate measurements
				  halfmsdelay(frequency);
				}
     		laser_measurement = laser_measurement/samp_per_ori; //average measurements
			  seveseg_main(laser_measurement);    // output to sevenseg
				laser_measurement = 0;        // reset variable

			// may need to check if all the calculation and displayng takes longer than servo delay
    	printsensor();
    }
  }
}
 /*  	
  h_final = h/2;
  h_angle = h_final*increment;
  height = 2*tan(h_angle)*central;
  if ( laser_measurement <= 1.5){
    w += 1;
  }
  w_final = w/2;
  w_angle = w_final*increment;
  width = 2*tan(h_angle)*central;
  } 
//area = width*height;
}
 */


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
 
      for (j = 0; j <= step_t; j++){      //< or <= steps
        if(checktai ==0){
          current_duty_t = min_duty_t+duty_increment*j;
          PWMDTY45 = current_duty_t;
          PWMCNT45 = 0x00; //clear register      
        } 
        else{
          current_duty_t = max_duty_t-duty_increment*j;
          PWMDTY45 = current_duty_t;
          PWMCNT45 = 0x00; //clear register        
        } 
        for (k = 0; k <= samp_per_ori; k++){                  // get distance data from laser  take multiple scan up to user
          converted = laser_main();                           //run laser measurement
          laser_measurement = laser_measurement + converted;  //accumulate measurements
          halfmsdelay(frequency);
        }
        laser_measurement = laser_measurement/samp_per_ori; //average measurements
        seveseg_main(laser_measurement);    // output to sevenseg
        laser_measurement = 0;        // reset variable

      // may need to check if all the calculation and displayng takes longer than servo delay
      printsensor();
    }
    if(checktai == 0){
    checktai = 1;
    } 
    else{
    checktai = 0;
    }
  