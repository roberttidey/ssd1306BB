/*
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @created: 2014-08-12
 * @author: Neven Boyanov
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 * 
 * Modified by Tejashwi Kalp Taru, with the help of TinyI2C (https://github.com/technoblogy/tiny-i2c/)
 * Modified code available at: https://github.com/tejashwikalptaru/ssd1306xled
 */

// ----------------------------------------------------------------------------
#ifdef __AVR_ATtiny85__
	#include <stdlib.h>
	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <I2CTinyBB.h>
#else
	#include <Wire.h>
#endif

#include "ssd1306BB.h"
#include "ssd1306fontsBB.h"

const uint8_t ssd1306_init_sequence[] PROGMEM  = {	// Initialization Sequence
	0xAE,			// Set Display ON/OFF - AE=OFF, AF=ON
	0xD5, 0xF0,		// Set display clock divide ratio/oscillator frequency, set divide ratio
	0xA8, 0x3F,		// Set multiplex ratio (1 to 64) ... (height - 1)
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0x40 | 0x00,	// Set start line address, at 0.
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, 10=Page, 11=Invalid
	0xA0 | 0x01,	// Set Segment Re-map
	0xC8,			// Set COM Output Scan Direction
	0xDA, 0x12,		// Set COM Pins Hardware Configuration - 128x32:0x02, 128x64:0x12
	0x81, 0xFF,		// Set contrast control register
	0xD9, 0x22,		// Set pre-charge period (0x22 or 0xF1)
	0xDB, 0x40,		// Set Vcomh Deselect Level - 0x00: 0.65 x VCC, 0x20: 0.77 x VCC (RESET), 0x30: 0.83 x VCC
	0xA4,			// Entire Display ON (resume) - output RAM to display
	0xA6,			// Set Normal/Inverse Display mode. A6=Normal; A7=Inverse
	0x2E,			// Deactivate Scroll command
	0xAF,			// Set Display ON/OFF - AE=OFF, AF=ON
};

SSD1306Device::SSD1306Device(void){}

uint8_t  i2cBuff[40];

//i2c write using either Wire of I2CBB library
void SSD1306Device::i2cw(uint8_t iaddr, uint8_t *data, uint8_t iLen) {
	#ifdef __AVR_ATtiny85__
		I2CWrite(iaddr, data, iLen);
	#else
		uint8_t i;
		Wire.beginTransmission(iaddr);
		for(i = 0; i < iLen; i++) {
			Wire.write(i2cBuff[i]);
		}
		Wire.endTransmission();
	#endif
}

void SSD1306Device::ssd1306_init(uint8_t sda, uint8_t scl, uint8_t saddr, uint8_t delay) {
	uint8_t i;
	oledAddr = saddr;
	#ifdef __AVR_ATtiny85__
		I2CInit(sda, scl, delay);
	#else
		Wire.begin();
		Wire.setClock(delay * 100000);
	#endif
	i2cBuff[0] = SSD1306_COMMAND;
	for (i = 0; i < sizeof(ssd1306_init_sequence); i++) {
		i2cBuff[i+1] = pgm_read_byte(&ssd1306_init_sequence[i]);
	}
	i2cw(oledAddr, i2cBuff, sizeof(ssd1306_init_sequence) + 1);
	ssd1306_fillscreen(0);
}

void SSD1306Device::ssd1306_sleep(uint8_t s) {
	i2cBuff[0] = SSD1306_COMMAND; 
	i2cBuff[1] = s ? 0xAE : 0xAF;
	i2cw(oledAddr, i2cBuff, 2);
}

void SSD1306Device::ssd1306_fillscreen(uint8_t fill) {
	uint16_t i;
	ssd1306_setpos(0, 0);
	i2cBuff[0] = SSD1306_DATA; 
	for (i = 1; i < 33; i++) {
		i2cBuff[i] = fill;
	}
	for (i = 0; i < 32; i++) {
		i2cw(oledAddr, i2cBuff, 33);
	}
}

void SSD1306Device::ssd1306_flipscreen(uint8_t flip) {
	i2cBuff[0] = SSD1306_COMMAND; 
	i2cBuff[1] = 0xA0; 
	i2cBuff[2] = flip ? 0xC0 : 0xC8; 
	i2cw(oledAddr, i2cBuff, 3);
}

void SSD1306Device::ssd1306_setpos(uint8_t x, uint8_t y) {
	xpos = x;
	ypos = y;
	i2cBuff[0] = SSD1306_COMMAND; 
	i2cBuff[1] = 0xb0 | (y & 0x07);
	i2cBuff[2] = 0x10 | ((x & 0xf0) >> 4); 
	i2cBuff[3] = x & 0x0f; 
	i2cw(oledAddr, i2cBuff, 4);
}

void SSD1306Device::ssd1306_setscale(uint8_t s) {
	scale = s;
}

void SSD1306Device::ssd1306_charInt(char ch, uint8_t shift, uint16_t offset) {
#if FONT < 2
	uint8_t i,j;
	unsigned char f;	
	uint8_t c = ch - 32;
	i2cBuff[0]  = SSD1306_DATA;
	j = 1;
	for (i= 0; i < FONT_WIDTH; i++) {
		f = (unsigned char)pgm_read_byte(&ssd1306xled_font[c * FONT_WIDTH + i + offset]);
		switch(scale) {
			case 0: i2cBuff[j] = f; j++;
					break;
			case 1: f = (unsigned char)pgm_read_byte(&nibbleDouble[(f >> shift) & 0x0f]);
					i2cBuff[j] = f; j++;
					i2cBuff[j] = f; j++;
					break;
			case 2: f = (unsigned char)pgm_read_byte(&diBitQuad[(f >> shift) & 0x03]);
					i2cBuff[j] = f; j++;
					i2cBuff[j] = f; j++;
					i2cBuff[j] = f; j++;
					i2cBuff[j] = f; j++;
					break;
		}
	}
	i2cw(oledAddr, i2cBuff, j);
#endif
}

void SSD1306Device::ssd1306_char(char ch) {
	#if FONT == 0
		switch(scale) {
			case 0: ssd1306_charInt(ch, 0, 0);
					break;
			case 1: ssd1306_charInt(ch, 0, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 4, 0);
					ssd1306_setpos(xpos+12, ypos-1);
					break;
			case 2: ssd1306_charInt(ch, 0, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 2, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 4, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 6, 0);
					ssd1306_setpos(xpos+24, ypos-3);
					break;
		}
	#elif FONT == 1
		switch(scale) {
			case 0: ssd1306_charInt(ch, 0, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 0, FONT_TABLEOFFSET);
					ssd1306_setpos(xpos+8, ypos-1);
					break;
			case 1: ssd1306_charInt(ch, 0, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 4, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 0, FONT_TABLEOFFSET);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 4, FONT_TABLEOFFSET);
					ssd1306_setpos(xpos+16, ypos-3);					break;
			case 2: ssd1306_charInt(ch, 0, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 2, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 4, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 6, 0);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 0, FONT_TABLEOFFSET);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 2, FONT_TABLEOFFSET);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 4, FONT_TABLEOFFSET);
					ssd1306_setpos(xpos, ypos+1);
					ssd1306_charInt(ch, 6, FONT_TABLEOFFSET);
					ssd1306_setpos(xpos+32, ypos-7);
					break;
		}
	#endif
}

void SSD1306Device::ssd1306_string(char *s) {
#if FONT < 2
	while (*s) {
		ssd1306_char(*s++);
	}
#endif
}

void SSD1306Device::ssd1306_string(uint8_t x, uint8_t y, char *s) {
#if FONT < 2
	ssd1306_setpos(x, y);
	ssd1306_string(s);
#endif
}

void SSD1306Device::ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]){
	uint16_t j = 0;
	uint8_t y, x;
	uint8_t i;
	
	i2cBuff[0] = SSD1306_DATA;
	if (y1 % 8 == 0) y = y1 / 8;
	else y = y1 / 8 + 1;
	for (y = y0; y < y1; y++) {
		ssd1306_setpos(x0,y);
		i = 1;
		for (x = x0; x < x1; x++) {
			i2cBuff[i] = pgm_read_byte(&bitmap[j++]);
			//write in blocks of 32 bytes until end
			if((i++ == 32) || (x == (x0 -1))) {
				i2cw(oledAddr, i2cBuff, i);
				i = 1;
			}
		}
	}
}

SSD1306Device SSD1306;

// ----------------------------------------------------------------------------