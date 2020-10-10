//demo
//R.J.Tidey 5 Oct 2020
//Note text co-ordinates are by pixel in x direction and by 8 pixels in Y direction
#include <Arduino.h>

#include <ssd1306BB.h>
#define SSD1306_SCL		1		// SCL
#define SSD1306_SDA		0		// SDA
#define SSD1306_SA		0X3C	// Slave address
#define SSDI2C_DELAY	4		// sets i2c speed

// // default PIN configuration is as below
// // VCC ---- vcc
// // GND ---- gnd
// // SCL ---- pb1
// // SDA ---- pb0
#define BLANK "                "
#define INTERVAL 1000

int loopCount = 0;
char strCount[6];

void displayInit() {
	SSD1306.ssd1306_fillscreen(0);
	SSD1306.ssd1306_string(0,6,BLANK);
	SSD1306.ssd1306_string(0,0,"Line 1");
	SSD1306.ssd1306_string(0,2,"Line 2");
	SSD1306.ssd1306_string(0,4,"Line 3");
	SSD1306.ssd1306_string(0,6,"Count");
}


void setup() {
	delay(400);
	SSD1306.ssd1306_init(SSD1306_SDA, SSD1306_SCL, SSD1306_SA, SSDI2C_DELAY);
	displayInit();
}

void loop() {
	itoa(loopCount, strCount, 10);
	SSD1306.ssd1306_string(64,6,strCount);
	loopCount++;
	if(loopCount > 9999) loopCount = 0;
	delay(INTERVAL);
}