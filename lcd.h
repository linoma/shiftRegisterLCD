#ifndef LLCD_h
#define LLCD_h

#include <inttypes.h>
#include "Print.h"
#include "Arduino.h"

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class LLCD:public Print{
public:
	LLCD(int,int,int);
	void init();
	virtual size_t write(uint8_t);
	void display();
	void clear();
	void noDisplay();
	void home();
	void setCursor(uint8_t col, uint8_t row);
	void noCursor();
	void cursor();
	void noBlink();
	void blink();
	void scrollDisplayLeft(void);
	void scrollDisplayRight(void);
	void leftToRight(void);
	void rightToLeft(void);
	void autoscroll(void);
	void noAutoscroll(void);
	void createChar(uint8_t location, uint8_t charmap[]);
protected:
	void send(uint8_t, uint8_t);
	void write4bits(uint8_t value);
	void command(uint8_t value);
	uint8_t _clock_pin;
	uint8_t _data_pin;
	uint8_t _latch_pin;	
	
	uint8_t _rs_pin; // LOW: command.  HIGH: character.
	uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
	uint8_t _enable_pin; // activated by a HIGH pulse.  
	
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
		
	unsigned char _sr_value,_enable_mask,_rs_mask;
};

#endif
