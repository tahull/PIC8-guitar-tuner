#include "ssd1306_oled.h"




void I2C1_WriteNBytes(i2c1_address_t address, uint8_t* data, size_t len)
{
    while(!I2C1_Open(address)); // sit here until we get the bus..
    I2C1_SetBuffer(data,len);
    I2C1_SetAddressNackCallback(NULL,NULL); //NACK polling?
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close()); // sit here until finished.
}

void ssd1306_init(void){
    uint8_t init_cmd1[] = {0x00,0xA8,0x3F,0xD3,0x00,0x40,0xA1,0xC8,0xDA,0x12};
    //split up the command chunks
    uint8_t init_cmd2[] = {0x00,0x81,0x7F,0xA4,0xA6,0xD5,0x80,0x8D,0x14,0x20,0x00,0xAF};
    
    I2C1_WriteNBytes(0x3C,init_cmd1,sizeof(init_cmd1));
    I2C1_WriteNBytes(0x3C,init_cmd2,sizeof(init_cmd2));
    
    //per data sheet, setup ssd1306 oled init commands
    //set mux ratio A8, 3F
    //set display offset D3, 00
    //set display start line 40
    //set segment remap A0 or A1
    //set com out put scan direction normal/remap (right side up or upside down) C0 or C8
    //set com pins hardware config DA, 02
    //set contrast control 81, 7F
    //disable entire display on A4
    //set normal display A6
    //set osc freq D5 80
    //enable charge pump regulator 8D, 14
    //display on AF
}

void ssd1306_clr(uint8_t startx, uint8_t starty, uint8_t endx, uint8_t endy){    
    //uint8_t clrscreen[] = {0x40,0x00,0x00,0x00,0x00,0x00};
    uint8_t clrscreen[] = {0x40,0x00};    
    for(uint8_t i = starty;i<endy;i++){
        ssd1306_gotoxy(startx,i);
        for(uint8_t j = startx; j < endx;j++){            
            I2C1_WriteNBytes(0x3C,clrscreen,sizeof(clrscreen));
        }
    }
}

void ssd1306_gotoxy(uint8_t x,uint8_t y){    
    uint8_t init_cmd[] = {0x00,0x21,x,127,0x22,y,7};
    I2C1_WriteNBytes(0x3C,init_cmd,sizeof(init_cmd));
}
