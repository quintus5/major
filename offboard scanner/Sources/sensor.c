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
//
#include "sensor.h"
#include "iic.h"  
#include "pll.h"
#include "sci1.h"
#include "math.h"
#include "l3g4200.h"  // register's definitions    ( not used by ed )
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

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

unsigned int edge1,diff,overflow;
unsigned long int pulse_width;




void getsensordata(void) {

	PLL_Init();  // make sure we are runnign at 24 Mhz
 
 
	EnableInterrupts;

//This program will send the gyro, accelerometer and magnetometer data
// to a serial port
// You can connect the serial port, set it to 9600 bauds 

 SCI1_Init(BAUD_9600);   // capped at 9600, if PLL inactive (4 MHz bus)
 SCI1_OutString("Program Starting ");      // should display this
 iicinit();
 
// gyro_test(); // make sure a l3g is connected
 
  gyro_init();     // l3g4200 setup
  accel_init();
  magnet_init();
 
// L3G4200d Gyro;
 l3g4200d_getrawdata( &gxraw, &gyraw, &gzraw) ;        // read data
 SCI1_OutString(" Gyro heaging: ");
// SCI1_OutUDec(());
 SCI1_OutString("\r\n");
  
 // ADCL345 Accelerometer
 adxl345_getrawdata( &axraw, &ayraw, &azraw) ;        // read data    
 SCI1_OutString(" Adegree:"); 
 SCI1_OutUDec((signed short) accel_deg);     
 SCI1_OutString("\r\n");

 // HM5883_magnetometer
 hm5883_getrawdata(&mxraw, &myraw, &mzraw);
 SCI1_OutString(" Mdegree:"); 
 SCI1_OutUDec((signed short) magnet_deg);     
 SCI1_OutString("\r\n");       
}
//   ******************  END Main   *****************



/************************calculation************************/

/*
void magnet_cal(int *mxraw,int *myraw, int *mzraw){  //magnetometer calculation
    double xg,yg,zg;
   
   		xg = *mxraw /rate;  //covert counts to Gauss's

		yg = *myraw /rate;
		
		zg = *mzraw /rate;
		
  //	magnet_deg = atan2(yg,sqrt(zg*zg+xg*xg)) * 180/ PI;
 
  	// No tilt compensation
  //	heading=noTiltCompensate(yg,xg);
  	
  	// Tilt compensation
	heading=tiltCompensate(xg,yg, zg, ayraw,axraw);
	declinationAngle = (11.0+(37.0/60.0))/(180/PI);
	heading +=	declinationAngle;
  	heading=correctAngle(heading);
  	magnet_deg=heading*180/PI;    
    
} */        
/*
int gyro_cal(int *gxraw, int *gyraw, int *gzraw){  //gyro calculation
	double x_dps, y_dps, z_dps;
	int x_offset,y_offset, z_offset;
	x_offset = 32;  //26
	y_offset = 65507;  //65501
	z_offset = 39;     //33
	
	x_dps = (*gxraw - x_offset)*0.00875;
	y_dps = (*gyraw - y_offset)*0.00875;
	z_dps = (*gzraw - z_offset)*0.00875;
	
	gx_deg = x_dps * pulse_width;
	gy_deg = y_dps * pulse_width;
	gz_deg = z_dps * pulse_width;
}
  */
/************************calculation************************/
//correct angle
float correctAngle(float heading)
{
	if(heading<0)
	{
		heading += 2*_M_PI;
	}
	if(heading>2*_M_PI)
	{
		heading -= 2*_M_PI;
	}
	return heading;
}


// No tilt compensation
/*float noTiltCompensate(double *yg,double *xg)
{
  float heading = atan2(yg, xg);
  return heading;
}
*/
/* Tilt compensation
float tiltCompensate(double xg,double yg, double zg, int *ayraw,int *axraw)
{
  // Pitch & Roll 
  
  float roll;
  float pitch;
  double ayg,axg;
  ayg = *ayraw * range;
  axg = *axraw * range;
  roll = asin(ayg);
  pitch = asin(-axg);

  if (roll > 0.78 || roll < -0.78 || pitch > 0.78 || pitch < -0.78)
  {
    return -1000;
  }
  
    // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);  
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);
  
  // Tilt compensation
 //float Xh = xg * cosPitch + zg * sinPitch;
//float Yh = xg * sinRoll * sinPitch + yg * cosRoll - zg * sinRoll * cosPitch;
 
float Xh = yg * sinRoll * sinPitch + xg * cosRoll - zg * sinRoll * cosPitch;
float Yh = yg * cosPitch + zg * sinPitch;
 
  float heading = atan2(Yh, Xh);
    
  return heading;
}
*/

/*****************/

// Magnetometer

void magnet_init(void){
  
  int  res1; 
  res1=iicstart(magnet_wr);
  res1=iictransmit(HM5883_MODE_REG );  // 
  res1=iictransmit(0x00 );
  iicstop(); 
 
}


void magnet_test(void){
  
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
	magnet_cal_notilt(mxraw,mzraw);
}  

void magnet_cal_notilt(int *mxraw, int *mzraw){
  heading = atan2(*mxraw,*mzraw)*180/_M_PI;
  if(heading < 0){
    heading = 360-heading;
  }
}



/*accelerometer*/
void accel_test(void){}


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
	accele_cal(axraw,ayraw,azraw);
}  
  

void accele_cal(int *axraw, int *ayraw, int *azraw){    //accelerometer calculation    
	  
	  accel_deg = 90 - atan2(*axraw,sqrt(pow(*ayraw,2)+pow(*azraw,2)))*180/_M_PI;
	  if(accel_deg < 0){
	  	accel_deg *= -1;
	  }
}
 

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
	 //gyro_cal(gxraw,gyraw,gzraw);
}
