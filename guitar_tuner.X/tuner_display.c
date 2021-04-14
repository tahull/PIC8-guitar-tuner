#include "tuner_display.h"

// TODO alternate tunings
static uint16_t notes[] = {E2, A2, D3, G3, B3, E4};
static uint16_t trans[] = {680, 960, 1280, 1710, 2210, 2880, 3700};

/* Given a frequency, compare with a tuning table, format and output.
 * output to ssd1306 oled
 */
void tuner_display(uint16_t f){
    uint8_t string_to_char[6] = {75,55,70,85,60,75}; //E, A, D, G, B, E
    int16_t x,y;
    int8_t b;
    //find the tuning range that the frequency 'f' falls into
    for(uint8_t i = 0; i < 6; i++){
        //if frequency is between lower_range and upper_range
        if(f >= trans[i] && f < trans[i+1]){
            ssd1306_draw_char(0,0,90, 2,true); //draw "f"
            ssd1306_draw_char(11,0,95, 2,true); //draw "="

            tuner_display_freq(f);
            ssd1306_draw_char(49,2,string_to_char[i], 6,true);

            x = (int16_t)(f - notes[i]);
            //within "accurate" tuning range
            if(abs(x) <= TUNING_ACCURACY){
                tuner_display_bars(0);
            }
            else{//too high or too low
                if(f > notes[i])
                    b = 1;
                else
                    b = -1;
                
                if(abs(x) >= TUNING_RANGE)
                    y = TUNING_INDICATORS*(b);
                else//distance to target frequency is above minimum range. draw more bars
                    y = (x/TUNING_POINTS)+b;
                //printf("f: %u m: %d diff: %d y: %d\n",f, TUNING_POINTS, x, y);
                tuner_display_bars((int8_t)y);
            }
            break;
        }
    }
}

void tuner_display_mode(uint8_t string_id){
    uint8_t string_to_char[6] = {75,55,70,85,60,75}; //E, A, D, G, B, E
    ssd1306_clr(106,0,127,1);

    if(string_id == 6){ // display A for auto range mode
        ssd1306_draw_char(118,0,55, 2,true); //draw "A"
        //ssd1306_draw_char(0,0,105, 2,true); //draw "u"
        //ssd1306_draw_char(0,0,110, 2,true); //draw "t"
        //ssd1306_draw_char(0,0,115, 2,true); //draw "o"
    }
    else{ // display user selected string to tune
        ssd1306_draw_char(106,0,string_to_char[string_id], 2,true);

        if(string_id < 2)
            ssd1306_draw_char(118,0,10, 2,true);
        else if(string_id < 5)
            ssd1306_draw_char(118,0,15, 2,true);
        else
            ssd1306_draw_char(118,0,20, 2,true);
    }
}

//num 24 - 4*12 or 5*12
void tuner_display_freq(uint16_t f){
    uint8_t pos,val, deci = false;

    ssd1306_clr(22,0,100,1);

    if(f > 999)
        pos = 6*(SSD1306_CHAR_WIDTH*2+1);
    else
        pos = 5*(SSD1306_CHAR_WIDTH*2+1);

    while(f){
        val = f%10;
        f /=10;
        ssd1306_draw_char(pos,0,num_to_char[val], 2,true);
        pos -= (SSD1306_CHAR_WIDTH*2+1);
        if(!deci){
            deci = true;
            ssd1306_draw_char(pos,0,100, 2,true);
            pos -= (SSD1306_CHAR_WIDTH*2+1);
        }
    }
}

void tuner_display_bars(int8_t val){
    uint8_t pos;
    ssd1306_clr(0,2,48,7);
    ssd1306_clr(80,2,127,7);

    if (val == 0){ //center tune
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
                pos -= (SSD1306_CHAR_WIDTH*2+1);
                val++;
            }
            else{
                pos += (SSD1306_CHAR_WIDTH*2+1);
                val--;
            }
        }
    }
}

// UART output for testing
#ifdef TUNE_DISPLAY
void tuner_display_uart(uint16_t f){
    uint8_t note_name[][3] = {"E2","A2","D3","G3","B3","E4"};
    //uint16_t trans[] = {E2-(A2-E2)/2, E2+(A2-E2)/2, A2+(D3-A2)/2, D3+(G3-D3)/2, G3+(B3-G3)/2, B3+(E4-B3)/2, E4+(E4-B3)/2};
    
    //find the tuning range that the frequency 'f' falls into
    for(uint8_t i = 0; i < 6; i++){
        //if frequency is between lower_range and upper_range
        if(f >= trans[i] && f < trans[i+1]){
#ifdef TUNE_DISPLAY_VERBOSE
            printf("found: %u.%u target: %u.%u between: %u to %u\n", \
                    (uint16_t)(f/10),(uint16_t)(f%10), \
                    (uint16_t)(notes[i]/10),(uint16_t)(notes[i]%10),\
                    (uint16_t)(trans[i]/10),(uint16_t)(trans[i+1]/10));
#endif
            //check if frequency 'f' is in an acceptable accuracy range for 'good tuning
            if(f >= notes[i] - (TUNING_ACCURACY+i) && f <= notes[i] + (TUNING_ACCURACY+i)){
                printf("---(%s)---\n",note_name[i]);
            }
            //pitch too high
            else if(f >= notes[i] + (TUNING_ACCURACY*3+i)){
                printf("---(%s)<<<\n",note_name[i]);
            }
            else if(f >= notes[i] + (TUNING_ACCURACY*2+i)){
                printf("---(%s)<<-\n",note_name[i]);
            }
            else if(f >= notes[i] + (TUNING_ACCURACY+i)){
                printf("---(%s)<--\n",note_name[i]);
            }
            //pitch too low
            else if(f <= notes[i] - (TUNING_ACCURACY*3+i)){
                printf(">>>(%s)---\n",note_name[i]);
            }
            else if(f <= notes[i] - (TUNING_ACCURACY*2+i)){
                printf("->>(%s)---\n",note_name[i]);
            }
            else if(f <= notes[i] - (TUNING_ACCURACY+i)){
                printf("-->(%s)---\n",note_name[i]);
            }
            break;
        } 
    }
}
#endif

