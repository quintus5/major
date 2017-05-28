//laser setup
void laser_setup(void){
	DDRT  = 0xFD; 			//set PT1 as input
	TSCR1 = 0x80;	        //enable timers
	TSCR2 = 0x04; 			//no overflow, prescaler 16 to ensure a maximum 40m measurement
	TIOS  = 0xFD; 			//set PT1 as input capture
}
 
//laser main
float laser_main(void){
	int Fraw;
	int Rraw;
	int Pulsewidth
	float Converted;

	TCTL4 = 0x04; 	//set PT1 to capture rising edge
	TFLG1 = 0x02; 	//clear C1F
	
	while (!(TFLG1 & TFLG1_C1F_MASK)); 	//detect if rising edge occurs
	Rraw  = TC1; 	//record if occurred
	TCTL4 = 0x08; 	//set PT1 to capture falling edge
	TFLG1 = 0x02; 	//clear C1F
	
	while (!(TFLG1 & TFLG1_C1F_MASK)); 	//detect if falling edge occurs
	Fraw  = TC1; 	//then record
	Pulsewidth = (Fraw - Rraw);
	Converted  = (Pulsewidth/1500); 	//display in mm
	return Converted;
}
