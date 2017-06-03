

#ifndef LCD_h
#define LCD_h

#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02

void CMDWRT4(unsigned char);
void DATWRT4(unsigned char);
void LCD_setup(void);
void LCD_string(unsigned char*);
void LCD_text(unsigned char);
void LCD_sensorval(unsigned char *);

#endif
