void resetservie(void){

	PWMPRCLK = 0x44; 	//Clock SA&SB = Fbus/16
	PWMSCLA  = 15; 		// clockSA / 2*15
	PWMSCLB  = 15; 		// clockSB / 2*15 
	PWMCLK   = 0xA0; 	// Use clockSA and clock SB for pp5 & pp7
	PWMPOL   = 0xF0; 	// high then low for polarity
	PWMCAE   = 0x00; 	// left aligned for PP5 and PP7
	PWMCTL   = 0xC0; 	// 16-bit PWM channel for pp5& pp7
	PWME 	   = 0xA0; 	// enable pp5&pp7
	PWMPER67 = 1000; 	// PWM frequency is 50Hz for pp7
	PWMDTY67 = 75; 		// 7.5% duty cycle for middle position
	PWMCNT67 = 0x00; 	// Clear the channel 7
	PWMPER45 = 1000; 	// PWM frequency is 50Hz for pp5
	PWMDTY45 = 75; 		// Set the duty cycle to 7.5% for middle position
	PWMCNT45 = 0x00; 	// Clear the channel 5
 
}


int angle_increased, duty_increased, current_duty;
int min_angle_h, max_angle_h, min_angle_t, max_angle_t, steps_h, steps_t, samples, laser_measurement = 0, sample_frequency_h, sample_frequency_t;

void motocontrol(void){
  
  for (i = 0; i < steps_h; i++){

    PWMDTY67 = ((min_angle_h+((max_angle_h-min_angle_h)*i)/steps_h)*(1/3000)+0.045)*1000; //set angle
    PWMCNT67 = 0x00; //clear register
    delay(); //delay function here
     
    for (j =0; j < steps_t; j++){ //< or <= steps
    	
      angle_increased = ((max_angle_h-min_angle_h)*j)/steps_t;
      duty_increased  = (min_angle_h+angle_increased)*(1/3000);
      current_duty    = (duty_increased+0.045)*1000;
      PWMDTY45        = current_duty;
       //PWMDTY45 =((min_angle_h+((max_angle_h-min_angle_h)*j)/steps_t)*(1/3000)+0.045)*1000; //set angle
    	PWMCNT45        = 0x00; //clear register
    		
      for (k = 0; k < samples; k++){
        laser_main(); //run laser measurement
    	  laser_measurement = laser_measurement + converted; //accumulate measurements
  		}
      
     	laser_measurement = laser_measurement / samples; //average measurements
    	delay(1/sample_frequency_t); //delay according to required frequency
   	}
  }
}