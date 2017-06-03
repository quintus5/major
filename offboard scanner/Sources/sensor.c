// Demonstration functions for IIC to read inertial sensor values
//
// The program will send read values to the serial port.
// you need to connect the serial port to a terminal to verify operation
// port speed 9600 bauds
//
// Eduardo Nebot  29 January 2016
// Sensors implemented:
// Gryro L2g2400D  (3 axis )
// Accelerometer  ADXL345
// Magnetometer HM5883
// Laser Lidarlight:  the driver is working with version 1 but not with version 2
// Version 2 work in progress: 
// Laser Interface will be done by measuring the pulse PWM pulse width
// Last version: 29/1/16
// 
//  This version installed interrupts with the simple model
//  The user should create a project selecting small memory model
//  and minimal startup code. This form will not intiliase any variables !
//  Your program will need to intialize the variables !
//
// Resources used: This program is using Timer 6 for the sampling time
// 
// the iic drivers are using Timer 7. ( You cannot use this timer in your program)
// Do not change the prescaler. If you do you need to change some code in iic.c

#include "sensor.h"
#include "iic.h"  
#include "pll.h"
#include "math.h"
#include "l3g4200.h"  // register's definitions    ( not used by ed )
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sensor_on_lcd.h"

char buff[BUFF_SIZE];
int gxraw[BUFF_SIZE];
int gyraw[BUFF_SIZE],gzraw[BUFF_SIZE];	

int axraw[BUFF_SIZE];
int ayraw[BUFF_SIZE],azraw[BUFF_SIZE];	

int mxraw[BUFF_SIZE];
int myraw[BUFF_SIZE],mzraw[BUFF_SIZE];	

float heading;
float accel_deg, magnet_deg;
float gx_deg,gy_deg,gz_deg;

void getsensordata(void) {

	PLL_Init();  // make sure we are runnign at 24 Mhz 
	EnableInterrupts;
  // initialize i2c data bus
  iicinit();
  gx_deg = 2;
  gy_deg = 4;
 
  gyro_init();     // l3g4200 setup
  accel_init();
  magnet_init();
 
 l3g4200d_getrawdata( &gxraw, &gyraw, &gzraw) ;    // L3G4200d Gyro;
 adxl345_getrawdata( &axraw, &ayraw, &azraw) ;    // ADCL345 Accelerometer  
 hm5883_getrawdata(&mxraw, &myraw, &mzraw);    // HM5883_magnetometer   

 printsensor(accel_deg,gx_deg,gy_deg,heading);      
}

//***********************************************************|
//************************calculation************************|
//***********************************************************|

//***********************Magnetometer************************

void magnet_init(void){
  
  int  res1; 
  res1=iicstart(magnet_wr);
  res1=iictransmit(HM5883_MODE_REG );  // 
  res1=iictransmit(0x00 );
  iicstop(); 
 
}

void hm5883_getrawdata(int *mxraw, int *myraw, int *mzraw){
  
 uint8_t i = 0;
 uint8_t buff[6];
 int res1;
	
 res1=iicstart(magnet_wr);
 res1=iictransmit(HM5883_DATAX0 );
 res1= iicrestart(magnet_rd); 
 iicswrcv();
 
 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }
 
 buff[i]= iicreceivem1();
 buff[i+1]= iicreceivelast();

	*mxraw = ((buff[0] << 8) | buff[1]);
	*myraw = ((buff[2] << 8) | buff[3]);
	*mzraw = ((buff[4] << 8) | buff[5]);
//	magnet_cal(mxraw, myraw,mzraw);
	magnet_cal_notilt(mxraw[0],mzraw[0]);
   return;
}  

void magnet_cal_notilt(int mx, int mz){
  heading = atan2(mx,mz)*180/_M_PI;
  if(heading < 0){
    heading = 360-heading;
  }
  return;
}

//***********************************accelerometer************

void accel_init (void){
  
 int  res1; 
 res1=iicstart(accel_wr);
 res1=iictransmit(ADXL345_POWER_CTL );  //  
 res1=iictransmit(0x08 );
  
 res1=iictransmit(ADXL345_DATA_FORMAT );  // ; 
 res1=iictransmit(0x08 );
  
 iicstop();  
}


void adxl345_getrawdata(int *axraw, int *ayraw, int *azraw){
  
 uint8_t i = 0;
 uint8_t buff[6];
 int res1;
	
 res1=iicstart(accel_wr);
 res1=iictransmit(ADXL345_DATAX0 );
 res1= iicrestart(accel_rd); 
 iicswrcv();
 
 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }
 
 buff[i]= iicreceivem1();
 buff[i+1]= iicreceivelast();

	*axraw = ((buff[1] << 8) | buff[0]);
	*ayraw = ((buff[3] << 8) | buff[2]);
	*azraw = ((buff[5] << 8) | buff[4]);
	accele_cal(axraw[0],ayraw[0],azraw[0]);
}  
  

void accele_cal(int ax, int ay, int az){    //accelerometer calculation    
	  accel_deg = 90 - atan2(ax,sqrt(pow(ay,2)+pow(az,2)))*180/_M_PI;
}
 
//****************************gyroscope*****************************************
// test the precense of Gyro , should get 211 on return 
// 

void gyro_test(void) {
 int res1,who; 
 
 res1=iicstart(0xD2);
 res1=iictransmit(L3G4200D_WHO_AM_I);
  
 res1=iicrestart(0xD3);
 who=iicreceiveone();

}

 //  Gyro Initialisation
 void gyro_init (void) {
  
	 int  res1;
	 
	 res1=iicstart(gyro_wr);
	 res1=iictransmit(L3G4200D_CTRL_REG1);  // ; output data rate 100hz, cut-off 12.5Hz, Power up
	 res1=iictransmit(0x0f);
	 iicstop();  
 }
 
 
// Function to get a set of gyro data
// Eduardo Nebot,30 July 2015 
void l3g4200d_getrawdata(int *gxraw, int *gyraw, int *gzraw) {
 	uint8_t i = 0;
	uint8_t buff[6];
	int res1;
	
   res1=iicstart(gyro_wr);
   res1=iictransmit(L3G4200D_OUT_XYZ_CONT );
   res1= iicrestart(gyro_rd); 
 
 iicswrcv();
 
 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }
 
buff[i]= iicreceivem1();
buff[i+1]= iicreceivelast();

	*gxraw = ((buff[1] << 8) | buff[0]);
	*gyraw = ((buff[3] << 8) | buff[2]);
	*gzraw = ((buff[5] << 8) | buff[4]);
	 gyro_cal();
}

void gyro_cal(void){
}
