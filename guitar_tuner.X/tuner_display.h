/* 
 * File:   tuning.h
 * Author: tahull
 * display human readable information post-signal-processing
 */

#ifndef TUNING_H
#define	TUNING_H

#include <stdint.h>
#include <stdio.h>
#include "tuner_defs.h"
#include "ssd1306_oled.h"

#define TUNING_ACCURACY 4 // 4 -> +/- 0.4 hz accuracy
#define TUNING_ACCURACY2 TUNING_ACCURACY*2 //>.4 < .8
#define TUNING_ACCURACY3 TUNING_ACCURACY*3 // >.8 < 1.2
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
void tuner_display_string(uint8_t string_id);
void tuner_display_uart(uint16_t f);


#endif	/* TUNING_H */

