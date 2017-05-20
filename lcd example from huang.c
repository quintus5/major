#define LCD_DAT PORTK // Port K drives LCD data pins~ E~ and RS
#define LCD_DIR DDRK // Direction of LCD port
#define LCD_E 0x02 // E signal
#define LCD_RS 0x01 // RS signal
#define LCD_E_RS 0x03 // assert both E and RS signals


void main (void)
{
	char *msg1 = "hello world!";
	char *msg2 = "LCD is working!";
	openLCD();
	putsLCD(msg1);
	cmd2LCD(0xC0); // move cursor to 2nd row~ 1st column
	putsLCD(msg2);
	asm("swi");
	while(1);
}


void cmd2LCD (char cmd)
{
	char temp;
	temp = cmd; // save a copy of the command
	cmd &=0xF0; // clear out the lower 4 bits
	LCD_DAT &= (~LCD_RS); // select LCD instruction register
	LCD_DAT |= LCD_E; // pull E signal to high
	cmd >>= 2; // shift to match LCD data pins
	LCD_DAT = cmd | LCD_E; // output upper 4 bits~ E~ and RS
	asm ("nop"); // dummy statements to lengthen E
	asm ("nop"); // "
	asm ("nop");
	LCD_DAT &= (~LCD_E); // pull E signal to low
	cmd = temp & 0x0F; // extract the lower 4 bits
	LCD_DAT |= LCD_E; // pull E to high
	cmd <<= 2; // shift to match LCD data pins
	LCD_DAT = cmd | LCD_E; // output upper 4 bits~ E~ and RS
	asm("nop"); // dummy statements to lengthen E
	asm("nop"); // "
	asm("nop");
	LCD_DAT &= (~LCD_E); // pull E-clock to low
	delayby50us(1); // wait until the command is complete
}

void openLCD(void)
{
	LCD_DIR = 0xFF; // configure LCD_DAT port for output
	delayby1ms(100);
	cmd2LCD(0x28); // set 4-bit data~ 2-line display~ =37 font
	cmd2LCD(0x0F); // turn on display~ cursor~ blinking
	cmd2LCD(0x06); // move cursor right
	cmd2LCD(0x01); // clear screen~ move cursor to home
	delayby1ms(2); // wait until clear display command is complete
}

void putcLCD(char cx)
{
	char temp;
	temp = cx;
	LCD_DAT |= LCD_RS; // select LCD data register
	LCD_DAT |= LCD_E; // pull E signal to high
	cx &= 0xF0; // clear the lower 4 bits
	cx >>= 2; // shift to match the LCD data pins
	LCD_DAT = cx|LCD_E_RS; // output upper 4 bits~ E~ and RS
	asm("nop"); // dummy statements to lengthen E
	asm("nop"); // "
	asm("nop");
	LCD_DAT &= (~LCD_E); // pull E to low
	cx = temp & 0x0F; // get the lower 4 bits
	LCD_DAT |= LCD_E; // pull E to high
	cx <<= 2; // shift to match the LCD data pins
	LCD_DAT = cx|LCD_E_RS; // output lower 4 bits~ E~ and RS
	asm("nop"); // dummy statements to lengthen E
	asm("nop"); // "
	asm("nop");
	LCD_DAT &= (~LCD_E); // pull E to low
	delayby50us(1);
}

void putsLCD (char *ptr)
{
	while (*ptr) {
	putcLCD(*ptr);
	ptr++;
	}
}

void delayby1ms(int sec){

  int i;
  TSCR1 = 0b10010000;             // enable timer and fast flag clear
  TSCR2 = 0b00000011;            // 8 prescale
  TIOS  = 0b00010000;           // channel 4 output compare
  TC4 = TCNT + 3000;           // add 3000 cycle if input is 1;
  
  for(i = 0; i < sec; i++){       // perform multiple 1 ms delay
    while(!(TFLG1 & C4F_flag));  // if input > 1
    TC4 += 3000;    
  }
  TIOS &= ~0b00010000;
}

void delayby50us(int sec){

  int i;
  TSCR1 = 0b10010000;             // enable timer and fast flag clear
  TSCR2 = 0b00000011;            // 8 prescale
  TIOS  = 0b00010000;           // channel 4 output compare
  TC4 = TCNT + 20;           // add 3000 cycle if input is 1;
  
  for(i = 0; i < sec; i++){       // perform multiple 1 ms delay
    while(!(TFLG1 & C4F_flag));  // if input > 1
    TC4 += 20;    
  }
  TIOS &= ~0b00010000;
}