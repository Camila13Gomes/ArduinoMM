#include "LiquidCrystalSerial.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "Arduino.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystalSerial constructor is called).

LiquidCrystalSerial::LiquidCrystalSerial(uint8_t sh, uint8_t ds, uint8_t st) {
    _sh_pin = sh;
    _ds_pin = ds;
    _st_pin = st;

    _rs_pin = 1;
    _rw_pin = 255;
    _enable_pin = 3;

    _data_pins[0] = 4;
    _data_pins[1] = 5;
    _data_pins[2] = 6;
    _data_pins[3] = 7;
    _data_pins[4] = 0;
    _data_pins[5] = 0;
    _data_pins[6] = 0;
    _data_pins[7] = 0;
}

void LiquidCrystalSerial::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw,
                               uint8_t enable, uint8_t d0, uint8_t d1,
                               uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5,
                               uint8_t d6, uint8_t d7) {
    _rs_pin = rs;
    _rw_pin = rw;
    _enable_pin = enable;

    _data_pins[0] = d0;
    _data_pins[1] = d1;
    _data_pins[2] = d2;
    _data_pins[3] = d3;
    _data_pins[4] = d4;
    _data_pins[5] = d5;
    _data_pins[6] = d6;
    _data_pins[7] = d7;

    if (fourbitmode)
        _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

    begin(16, 1);
}

void LiquidCrystalSerial::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    pinMode(_sh_pin, OUTPUT);
    pinMode(_ds_pin, OUTPUT);
    pinMode(_st_pin, OUTPUT);

    delayMicroseconds(50000);
    output_595(_rs_pin, 0);
    output_595(_enable_pin, 0);
    if (_rw_pin != 255) {
        output_595(_rw_pin, 0);
    }

    // put the LCD into 4 bit or 8 bit mode
    if (!(_displayfunction & LCD_8BITMODE)) {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        write4bits_595(0x03);
        delayMicroseconds(4500);  // wait min 4.1ms

        // second try
        write4bits_595(0x03);
        delayMicroseconds(4500);  // wait min 4.1ms

        // third go!
        write4bits_595(0x03);
        delayMicroseconds(150);

        // finally, set to 4-bit interface
        write4bits_595(0x02);
    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        command(LCD_FUNCTIONSET | _displayfunction);
        delayMicroseconds(4500);  // wait more than 4.1ms

        // second try
        command(LCD_FUNCTIONSET | _displayfunction);
        delayMicroseconds(150);

        // third go
        command(LCD_FUNCTIONSET | _displayfunction);
    }

    // finally, set # lines, font size, etc.
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

void LiquidCrystalSerial::setRowOffsets(int row0, int row1, int row2,
                                        int row3) {
    _row_offsets[0] = row0;
    _row_offsets[1] = row1;
    _row_offsets[2] = row2;
    _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystalSerial::clear() {
    command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    delayMicroseconds(2000);    // this command takes a long time!
}

void LiquidCrystalSerial::home() {
    command(LCD_RETURNHOME);  // set cursor position to zero
    delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystalSerial::setCursor(uint8_t col, uint8_t row) {
    const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
    if (row >= max_lines) {
        row = max_lines - 1;  // we count rows starting w/0
    }
    if (row >= _numlines) {
        row = _numlines - 1;  // we count rows starting w/0
    }

    command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystalSerial::noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalSerial::display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystalSerial::noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalSerial::cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystalSerial::noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalSerial::blink() {
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystalSerial::scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystalSerial::scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystalSerial::leftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystalSerial::rightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystalSerial::autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystalSerial::noAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystalSerial::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7;  // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystalSerial::command(uint8_t value) { send_595(value, 0); }

inline size_t LiquidCrystalSerial::write(uint8_t value) {
    send_595(value, 1);
    return 1;  // assume sucess
}

/************ low level data pushing commands for Serial **********/

void LiquidCrystalSerial::send_595(uint8_t value, uint8_t mode) {
    output_595(_rs_pin, mode);

    // if there is a RW pin indicated, set it low to Write
    if (_rw_pin != 255) {
        output_595(_rw_pin, 0);
    }

    if (_displayfunction & LCD_8BITMODE)
        ;
    else {
        write4bits_595(value >> 4);
        write4bits_595(value);
    }
}

void LiquidCrystalSerial::pulseEnable_595(void) {
    output_595(_enable_pin, 0);
    delayMicroseconds(1);
    output_595(_enable_pin, 1);
    delayMicroseconds(1);  // enable pulse must be >450ns
    output_595(_enable_pin, 0);
    delayMicroseconds(100);  // commands need > 37us to settle
}

void LiquidCrystalSerial::write4bits_595(uint8_t value) {
    for (int i = 0; i < 4; i++) {
        output_595(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable_595();
}

void LiquidCrystalSerial::output_595(uint8_t pin, bool state) {
    static long val = 0;
    bitWrite(val, 7 - pin, state);
    for (int i = 0; i < 8; i++) {
        digitalWrite(_ds_pin, bitRead(val, i));
        digitalWrite(_sh_pin, 1);
        digitalWrite(_sh_pin, 0);
    }
    digitalWrite(_st_pin, 1);
    digitalWrite(_st_pin, 0);
}
