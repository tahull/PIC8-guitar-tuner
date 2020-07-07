#include "ssd1306_oled.h"

//character bytes for oled cherry picked and modified from adafruit gfx lib default font
static const uint8_t char5x7[] = {
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

static uint8_t disp_buf[6] ={0};
//mapping for 0-9 chars
static const uint8_t num_to_char[10] = {0,5,10,15,20,25,30,35,40,45};

//private functions for ssd1306
static void ssd1306_clr_buf(uint8_t size);

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

    //clear display
    ssd1306_clr(0,0,128,8);
}

void ssd1306_clr(uint8_t startx, uint8_t starty, uint8_t endx, uint8_t endy){    
    //uint8_t clrscreen[] = {0x40,0x00,0x00,0x00,0x00,0x00};
    uint8_t clr[] = {0x40,0x00};    
    for(uint8_t i = starty;i<endy;i++){
        ssd1306_gotoxy(startx,i);
        for(uint8_t j = startx; j < endx;j++){            
            I2C1_WriteNBytes(0x3C,clr,sizeof(clr));
        }
    }
}

void ssd1306_gotoxy(uint8_t x,uint8_t y){    
    uint8_t init_cmd[] = {0x00,0x21,x,127,0x22,y,7};
    I2C1_WriteNBytes(0x3C,init_cmd,sizeof(init_cmd));
}


void ssd1306_putchar(uint8_t* buf, uint8_t x, uint8_t y, uint8_t size, uint8_t ow){
    uint8_t charbuf[2] = {0x40,0};

    ssd1306_gotoxy(x, y); 
    for(uint8_t i = 0;i < size;i++){
        if( (buf[i] != 0 && ow == false)|| ow == true){
            charbuf[1] = buf[i];
            I2C1_WriteNBytes(0x3C,charbuf,2);
        }
    }
}

static void ssd1306_clr_buf(uint8_t size){
    for(uint8_t i = 0; i < size; i++)
        disp_buf[i] = 0;
}

void ssd1306_draw_char( int16_t x, int16_t y, uint8_t ch, uint8_t size,uint8_t ow) {
    for(int8_t i = 0; i < 5 ; i++ ){
        uint8_t line = 0;
        line = char5x7[ch+i];
        for(int8_t j = 0; j < 8 ; j++, line >>= 1 ){
            uint8_t starty = j*size;
            uint8_t startx = i*size;            
            uint8_t pos = (starty>>3);
            //1 fill buffer with bits expanded to "size"
            for(int8_t k = starty ; k < starty+size ; k++ ){ 
                if( line & 0x1 ){
                    uint8_t pixmask = (1 << ((k)&0x07)); 
                    for(uint8_t l = 0; l < size; l++){
                        disp_buf[l] |= pixmask;                        
                    }
                }
                //when 8bits are processed
                if(!((k+1)%8)){
                    //2 send full buffer to display
                    ssd1306_putchar(disp_buf,x+startx,y+pos,size,ow);
                    //3 clear buffer
                    ssd1306_clr_buf(size);
                }
            }
        }        
    }
}

//num 24 - 4*12 or 5*12
void ssd1306_disp_f(uint16_t f){
    uint8_t pos,val, deci = false;
    
    ssd1306_clr(24,0,128,2);
    
    if(f > 999)
        pos = 6*TOP_CHAR_WIDTH;
    else
        pos = 5*TOP_CHAR_WIDTH;
    
    while(f){
        val = f%10;
        f /=10;                       
        ssd1306_draw_char(pos,0,num_to_char[val], 2,true);
        pos -= TOP_CHAR_WIDTH;
        if(!deci){
            deci = true;            
            ssd1306_draw_char(pos,0,100, 2,true);
            pos -= TOP_CHAR_WIDTH;
        }
    }    
}

void ssd1306_disp_tune_bar(int8_t val){
    uint8_t pos;
    
    ssd1306_clr(0,2,48,8);
    ssd1306_clr(80,2,128,8);
    
    if (val == 0){
        ssd1306_draw_char(39,4,50, 3,false);
        ssd1306_draw_char(83,4,50, 3,false);        
    }
    else{
        if (val < 0)
            pos = 37;
        else
            pos = 83;

        while(val){    
            ssd1306_draw_char(pos,3,50, 4,false);
            if(val < 0){
                pos -= MAIN_CHAR_WIDTH;
                val++;
            }
            else{
                pos += MAIN_CHAR_WIDTH;
                val--;
            }
        }   
    }
}
