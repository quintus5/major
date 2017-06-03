#include "sensor_on_lcd.h"
#include "LCD.h"
#include "sevenseg.h"

unsigned int unitnum;
unsigned char unitdig;
 
unsigned int tenthnum;
unsigned char tenthdig;
 
unsigned int hundrednum;
unsigned char hundreddig;
 
           
unsigned char value_string[5];
const unsigned char numbers[19] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','F','F','F'};
   
void printsensor(int accelerometer, int gyroheading, int gyroelevation, int magnetometer){
        
        CMDWRT4(0xC0);            //set start posistion, home position
        halfmsdelay(1);
          
        Int_to_string2(accelerometer);      
        Int_to_string2(gyroheading);
        Int_to_string2(gyroelevation);
        Int_to_string2(magnetometer);

}
  /*
//convert int read from sensor to string form
void Int_to_string(int num) {
 
        //read each digital of the num
        if(num >= 100){
                hundrednum = num/100;
                tenthnum = (num/10)%10;
                unitnum = num%10;
        }
        else if(num >= 10){
                hundrednum = 0;
                tenthnum = num/10;
                unitnum = num%10;
        }
        else{
                hundrednum = 0;
                tenthnum = 0;
                unitnum = num;
        }
        
        //store it in the string
        hundreddig = numbers[hundrednum];
        tenthdig = numbers[tenthnum];
        unitdig = numbers[unitnum];
        
        value_string[0] = hundreddig;
        value_string[1] = tenthdig;
        value_string[2] = unitdig;
        value_string[3] = ' ';
        value_string[4] = 0x00;         //null character
       
	LCD_sensorval(value_string);        
}
*/
//convert int read from sensor to string form
void Int_to_string2(int num) {
 
        //read each digital of the num
        if(num >= 100){
                value_string[0] = numbers[num/100];
                value_string[1] = numbers[(num/10)%10];
                value_string[2] = numbers[num%10];
        }
        else if(num >= 10){
                value_string[0] = numbers[0];
                value_string[1] = numbers[num/10];
                value_string[2] = numbers[num%10];
        }
        else{
                value_string[0] = numbers[0];
                value_string[1] = numbers[0];
                value_string[2] = numbers[num];
        }
        
        value_string[3] = ' ';
        value_string[4] = 0x00;         //null character
       
        LCD_sensorval(value_string);        
}
