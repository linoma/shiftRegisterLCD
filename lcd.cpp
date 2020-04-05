#include "lcd.h"

#define SHIFTOUT()\
digitalWrite(_latch_pin,LOW);\
shiftOut(_data_pin, _clock_pin, MSBFIRST, _sr_value);\
digitalWrite(_latch_pin,HIGH);

LLCD::LLCD(int d,int c,int l){
	_data_pin = d;
	_clock_pin = c;
	_latch_pin = l;

//	_backlight = LCD_BL_PIN;

	_enable_pin=6;
	_rs_pin=7;
	
	_enable_mask = (1 << _enable_pin);
	_rs_mask = (1 << _rs_pin);
}

void LLCD::init(){
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	_displayfunction |= LCD_2LINE;
	
	pinMode(_data_pin, OUTPUT);
	pinMode(_clock_pin, OUTPUT);
	pinMode(_latch_pin, OUTPUT);	
	
	write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02); 	
    
	command(LCD_FUNCTIONSET | _displayfunction);  

// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
	display();

  // clear it off
	clear();

  // Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);   
}

void LLCD::display() {
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LLCD::clear()
{
	command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

size_t LLCD::write(uint8_t data){
	send(data,HIGH);
	return 1;
}

void LLCD::send(uint8_t data, uint8_t mode){
	if(mode)
		_sr_value |= _rs_mask;
	else
		_sr_value &= ~_rs_mask;
	SHIFTOUT()
	//digitalWrite(_rs_pin, mode);

	write4bits(data >> 4);
	write4bits(data & 0xF);
}

inline void LLCD::command(uint8_t value) {
	send(value,LOW);
}

void LLCD::write4bits(uint8_t value) {	
	_sr_value = (_sr_value & 0xF0) | (value & 0xF);
	SHIFTOUT()
	
	//digitalWrite(_enable_pin, LOW);
	_sr_value &= ~_enable_mask;
	SHIFTOUT()
	delayMicroseconds(1);    
		
	//digitalWrite(_enable_pin, HIGH);
	_sr_value |= _enable_mask;
	SHIFTOUT()
	delayMicroseconds(1);
	
	//digitalWrite(_enable_pin, LOW);	
	_sr_value &= ~_enable_mask;
	SHIFTOUT()
	delayMicroseconds(100);
}

void LLCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LLCD::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LLCD::setCursor(uint8_t col, uint8_t row)
{
  /*const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));*/
}

void LLCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LLCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LLCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LLCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LLCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LLCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LLCD::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LLCD::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LLCD::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LLCD::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LLCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}
