/* 
 * File:   tuner_display.h
 * Author: tahull
 * display human readable information post-signal-processing
 */

#ifndef TUNER_DISPLAY_H
#define	TUNER_DISPLAY_H

#include <stdint.h>
#include <stdio.h>
#include "tuner_defs.h"
#include "ssd1306_oled.h"

#define TUNING_ACCURACY 4 // 4 -> +/- 0.4 hz accuracy center
#define TUNING_RANGE    60      //tuning accuracy range 60 -> 6hz
#define TUNING_INDICATORS   4   //number of bars to draw on screen to indicate tunning accuracy
#define TUNING_POINTS   (TUNING_RANGE/TUNING_INDICATORS) //split up the tune range by the number of indicators

// standard tuning 440.0 hz
#define E2 824  //82.4 Hz
#define A2 1100 //110.0 Hz
#define D3 1468 //146.8 Hz
#define G3 1960 //196.0 Hz
#define B3 2469 //246.9 Hz
#define E4 3296 //329.6 Hz
    
void tuner_display(uint16_t f);
void tuner_display_mode(uint8_t string_id);
/* write a number to the oled screen on the top bar. there's no negative frequency
 */
void tuner_display_freq(uint16_t f);
/* write vertical lines to the screen on the main area to indicate guitar tuning
 * -4 to -1 = draw 4 to 1 bars to the left of center
 * 0 - draw two smaller bars one on each side of center ex (|E|) a correct tuned E note
 * 4 to 1 = draw that number of bars to the right of center
 */
void tuner_display_bars(int8_t val);

#ifdef TUNE_DISPLAY
void tuner_display_uart(uint16_t f);
#endif


#endif	/* TUNER_DISPLAY_H */

