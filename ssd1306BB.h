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
*/
#include <stdint.h>
#include <Arduino.h>

#ifndef SSD1306BB_H
#define SSD1306BB_H

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

// ----------------------------------------------------------------------------

class SSD1306Device
{
    public:
		SSD1306Device(void);
		void ssd1306_init(uint8_t sda, uint8_t scl, uint8_t saddr, uint8_t delay);
		void ssd1306_sleep(uint8_t s);
		void ssd1306_setpos(uint8_t x, uint8_t y);
		void ssd1306_setscale(uint8_t s);
		void ssd1306_fillscreen(uint8_t fill);
		void ssd1306_flipscreen(uint8_t flip);
		void ssd1306_char(char ch);
		void ssd1306_string(char *s);
		void ssd1306_string(uint8_t x, uint8_t y, char *s);
		void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);

	private:
		uint8_t oledAddr;
		uint8_t xpos;
		uint8_t ypos;
		uint8_t scale = 1;
		void ssd1306_charInt(char ch, uint8_t mode, uint16_t offset);
		void i2cw(uint8_t iaddr, uint8_t *data, uint8_t iLen);
};


extern SSD1306Device SSD1306;

// ----------------------------------------------------------------------------

#endif