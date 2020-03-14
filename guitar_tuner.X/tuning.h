/* 
 * File:   tuning.h
 * Author: tahull
 * display human readable information post-signal-processing
 */

#ifndef TUNING_H
#define	TUNING_H

#include <stdint.h>
#include <stdio.h>
#include "guitar_tuner.h"

#define TUNING_ACCURACY 15 // 15 -> +/- 1.5 hz accuracy
// standard tuning
#define E2 824  //82.4 Hz
#define A2 1100 //110.0 Hz
#define D3 1468 //146.8 Hz
#define G3 1960 //196.0 Hz
#define B3 2469 //246.9 Hz
#define E4 3296 //329.6 Hz
    
void tuner_display(uint16_t f);


#endif	/* TUNING_H */

