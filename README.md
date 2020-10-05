# SSD1306BB

-This library is a simple driver for SSD1306 based OLED screens.
-It is designed for small footprint targets like ATTiny85.
-It uses BitBang I2C to allow freedom of choice on GPIO used and avoid conflict between dedicated pin usage and hardware I2C

-One of 3 fonts may be selected by setting the FONT_TYPE in the font header file
-Default FONT 1 gives a 12x16 character doubled up from a 6x8 to minimise font definition table

-x positioning is by pixel
-y positioning is in multiples of 8. e.g. for FONT 1 there are 4 rows (y = 0,2,4,6)

### Functions
-void ssd1306_init(uint8_t sda, uint8_t scl, uint8_t saddr, uint32_t iClock);
-void ssd1306_setpos(uint8_t x, uint8_t y);
-void ssd1306_fillscreen(uint8_t fill);
-void ssd1306_flipscreen(uint8_t flip);
-void ssd1306_char(char ch);
-void ssd1306_string(char *s);
-void ssd1306_string(uint8_t x, uint8_t y, char *s);
-void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);
