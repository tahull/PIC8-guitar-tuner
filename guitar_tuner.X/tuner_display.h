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
#define TUNING_ACCURACY2 TUNING_ACCURACY*2
#define TUNING_ACCURACY3 TUNING_ACCURACY*3

// standard tuning 440.0 hz
#define E2 824  //82.4 Hz
#define A2 1100 //110.0 Hz
#define D3 1468 //146.8 Hz
#define G3 1960 //196.0 Hz
#define B3 2469 //246.9 Hz
#define E4 3296 //329.6 Hz
    
void tuner_display(uint16_t f);
void tuner_display_uart(uint16_t f);


#endif	/* TUNING_H */

