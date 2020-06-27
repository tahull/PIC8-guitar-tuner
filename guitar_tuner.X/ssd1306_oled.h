/* 
 * File:   ssd1306_oled.h
 * Author: tahull
 * Simple oled driver for a guitar tuner
 * 
 */

#ifndef SSD1306_OLED_H
#define	SSD1306_OLED_H

#include "mcc_generated_files/mcc.h" //include i2c driver

//write a chunk of data to i2c (copied from mcc's i2c master example)
void I2C1_WriteNBytes(i2c1_address_t address, uint8_t *data, size_t len);


//initialize i2c ssdc1306 oled with set of typical commands from datasheet
void ssd1306_init(void);   
//clear a rectangular area from oled screen from start xy, to end xy coordinates
void ssd1306_clr(uint8_t startx, uint8_t starty, uint8_t endx, uint8_t endy);
//set xy coordinates of collum position (0-127) and page (0-7) for a 128x64 pixel oled
void ssd1306_gotoxy(uint8_t x, uint8_t y);

//character bytes for oled cherry picked and modified from adafruit gfx lib default font
const uint8_t char5x7[] = {
    0x3E, 0x51, 0x49, 0x45, 0x3E,   //0  0
    0x00, 0x42, 0x7F, 0x40, 0x00,   //5  1
    0x42, 0x61, 0x51, 0x49, 0x46,   //10 2
    0x21, 0x41, 0x45, 0x4B, 0x31,   //15 3
    0x18, 0x14, 0x12, 0x7F, 0x10,   //20 4
    0x27, 0x45, 0x45, 0x45, 0x39,   //25 5
    0x3C, 0x4A, 0x49, 0x49, 0x30,   //30 6
    0x01, 0x71, 0x09, 0x05, 0x03,   //35 7
    0x36, 0x49, 0x49, 0x49, 0x36,   //40 8
    0x06, 0x49, 0x49, 0x29, 0x1E,   //45 9
    0xFF, 0xFF, 0x00, 0x00, 0x00,   //50 |
    0x7C, 0x12, 0x11, 0x12, 0x7C,   //55 A
    0x7F, 0x49, 0x49, 0x49, 0x36,   //60 B
    0x3E, 0x41, 0x41, 0x41, 0x22,   //65 C
    0x7F, 0x41, 0x41, 0x22, 0x1C,   //70 D
    0x7F, 0x49, 0x49, 0x49, 0x41,   //75 E
    0x7F, 0x09, 0x09, 0x09, 0x01,   //80 F
    0x3E, 0x41, 0x49, 0x49, 0x7A,   //85 G
    0x08, 0x7E, 0x09, 0x01, 0x02,   //90 f
    0x14, 0x14, 0x14, 0x14, 0x14,   //95 =
    0x00, 0x60, 0x60, 0x00, 0x00    //100 .
};


#endif	/* SSD1306_OLED_H */
