/* 
 * File:   ssd1306_oled.h
 * Author: tahull
 * Simple SSD1306 oled driver
 * oled size is 128x64
 */

#ifndef SSD1306_OLED_H
#define	SSD1306_OLED_H

#include "mcc_generated_files/mcc.h" //include i2c driver

#define SSD1306_CHAR_WIDTH 5

//mapping for 0-9 chars
const uint8_t num_to_char[10] = {0,5,10,15,20,25,30,35,40,45};

//write a chunk of data to i2c (copied from mcc's i2c master example)
void I2C1_WriteNBytes(i2c1_address_t address, uint8_t *data, size_t len);

//initialize i2c ssdc1306 oled with set of typical commands from datasheet
void ssd1306_init(void);   
//clear a rectangular area from oled screen from start xy, to end xy coordinates.
//X limits 0-127, Y limits 0-7
void ssd1306_clr(uint8_t startx, uint8_t starty, uint8_t endx, uint8_t endy);
//set xy coordinates of collum position (0-127) and page (0-7) for a 128x64 pixel oled
void ssd1306_gotoxy(uint8_t x, uint8_t y);

void ssd1306_putchar(uint8_t* buf, uint8_t x, uint8_t y, uint8_t size, uint8_t ow);

/* modified and simplified from adafruit gfx draw char, uses a 6 byte buffer instead of a 1024 full screen buffer
 * instead of writing to all pixels (1024 bytes) this will write up-to 6 bytes at a time
 * x - start x location
 * y - start y location
 * ch - index of the char buffer "char5x7[ch]"
 * size - expansion size. size 2 will make a 5x7 char into a 10x14
 * ow - on/off to clear, write blank pixels
 * TODO: error checking size limits
*/
void ssd1306_draw_char( uint8_t x, uint8_t y, uint8_t ch, uint8_t size,uint8_t ow); 

#endif	/* SSD1306_OLED_H */

