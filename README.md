# SSD1306BB

- This library is a simple driver for SSD1306 based OLED screens.
- It was designed for small footprint targets like ATTiny85.
- For ATtiny85 it uses BitBang I2C to allow freedom of choice on GPIO used and avoid conflict between dedicated pin usage and hardware I2C
- https://github.com/roberttidey/I2CTinyBB
- Will detect if other CPUs are used and then use the Wire library for I2C
- sda,scl for non ATtiny should be set to I2C compatible Wire pins in init call
- One of 2 fonts may be selected by setting the FONT_TYPE in the font header file
- FONT 0 is small 6x8 characters
- FONT 1 is 8x16 narrow characters to fit more horizontally
- Scale may be set to 1,2,4 to replicate pixels in both directions
- Default is FONT 0 and scale 2 to give 12 x 16 characters

- x positioning is by pixel
- y positioning is in multiples of 8. e.g. for FONT 0 Scale 2 there are 4 rows (y = 0,2,4,6)

- setsleep turns display on (0) and off(1) to minimise current

### Functions
- ssd1306_init(uint8_t sda, uint8_t scl, uint8_t saddr, uint32_t iClock);
- ssd1306_sleep(uint8_t s);
- ssd1306_setpos(uint8_t x, uint8_t y);
- ssd1306_setscale(uint8_t s);
- ssd1306_fillscreen(uint8_t fill);
- ssd1306_flipscreen(uint8_t flip);
- ssd1306_char(char ch);
- ssd1306_string(char *s);
- ssd1306_string(uint8_t x, uint8_t y, char *s);
- ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);
