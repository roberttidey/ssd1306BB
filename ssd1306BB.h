/*
 * SSD1306BB - Drivers for SSD1306 128x64 displays
 *
 * @created: 2020-10-05
 * @author: Bob Tidey
 *
 * https://github.com/roberttidey/SSD1306BB
 * 
 * Based on work by Neven Boyanov, Tejashwi Kalp Taru
 *
 * Uses BitBang I2C library to allow free pin definition for I2C
 * https://github.com/bitbank2/BitBang_I2C
*/
#include <stdint.h>
#include <Arduino.h>
#include <I2CTinyBB.h>

#ifndef SSD1306BB_H
#define SSD1306BB_H

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

// ----------------------------------------------------------------------------

class SSD1306Device
{
    public:
		SSD1306Device(void);
		void ssd1306_init(uint8_t sda, uint8_t scl, uint8_t saddr, uint8_t delayCount);
		void ssd1306_setpos(uint8_t x, uint8_t y);
		void ssd1306_fillscreen(uint8_t fill);
		void ssd1306_flipscreen(uint8_t flip);
		void ssd1306_char(char ch);
		void ssd1306_string(char *s);
		void ssd1306_string(uint8_t x, uint8_t y, char *s);
		void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);

	private:
//		BBI2C bbi2c;
		uint8_t oledAddr;
		uint8_t xpos;
		uint8_t ypos;
		void ssd1306_charInt(char ch, uint8_t mode, uint16_t offset);
};


extern SSD1306Device SSD1306;

// ----------------------------------------------------------------------------

#endif