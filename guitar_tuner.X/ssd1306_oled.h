/* 
 * File:   ssd1306_oled.h
 * Author: tahull
 * Simple oled driver for a guitar tuner
 * oled size is 128x64
 */

#ifndef SSD1306_OLED_H
#define	SSD1306_OLED_H

#include "mcc_generated_files/mcc.h" //include i2c driver

#define TOP_CHAR_WIDTH 12
#define MAIN_CHAR_WIDTH 12

//write a chunk of data to i2c (copied from mcc's i2c master example)
void I2C1_WriteNBytes(i2c1_address_t address, uint8_t *data, size_t len);

//initialize i2c ssdc1306 oled with set of typical commands from datasheet
void ssd1306_init(void);   
//clear a rectangular area from oled screen from start xy, to end xy coordinates
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

/* write a number to the oled screen on the top bar. there's no negative frequency
 */
void ssd1306_disp_f(uint16_t f);

/* write vertical lines to the screen on the main area to indicate guitar tuning
 * -4 to -1 = draw 4 to 1 bars to the left of center
 * 0 - draw two smaller bars one on each side of center ex (|E|) a correct tuned E note
 * 4 to 1 = draw that number of bars to the right of center
 */
void ssd1306_disp_tune_bar(int8_t val);

#endif	/* SSD1306_OLED_H */

