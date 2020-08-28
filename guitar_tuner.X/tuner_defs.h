/* 
 * File:   guitar_tuner.h
 * Author: tahull
 *
 */

#ifndef GUITAR_TUNER_H
#define	GUITAR_TUNER_H

#include <stdint.h>

//guitar tuner config
#define FS          4943    //sample frequency set by timmer interrupt rate
#define SAMPLE_SIZE 256     //size of signal sample array

#define ADCBITS     7
//voltage bias from amplifier circuit in volts
#define VBIAS       1.8     //1.8v
//voltage level/trigger point to start collecting samples
//set high enough so the algorithm isn't constantly processing noise
#define VTRIGGER    3.5 
//fvr positive reference voltage 4.096v
#define ADCVREF     4.096   
// adc offset. ex for 10 bit
// 4.096v/(2^10) = 4mv per bit. 1.8v(bias from voltage divider on op amp)/.004v = 450
#define ADCOFFSET (int16_t)(VBIAS/((double)ADCVREF/(1<<ADCBITS)))
#define TRIGGER_LEVEL (int16_t)(VTRIGGER/((double)ADCVREF/(1<<ADCBITS)))

// frequency limits. expected range for a guitar: 50 hz to 400 hz
#define F_MIN       50
#define F_MAX       400
#define T_MIN FS/F_MAX      //minimum period normalized to sample frequency
#define T_MAX FS/F_MIN      //maximum period normalized to sample frequency  

//ADC bit resolution 
#if ADCBITS > 8
typedef int16_t samp_t;
typedef uint16_t adc_t;
#else
typedef uint8_t adc_t;
#if ADCBITS < 8
typedef int8_t samp_t;
#else
typedef int16_t samp_t;
#endif
#endif

//debug. uncomment to print debug info
//#define RAW_SIGNAL_DEBUG    // Raw ADC sample buffer
//#define AMDF_DEBUG          // Processed amdf vals
//#define INTP_DEBUG          // Print interpolation debug info
//#define TUNE_DISPLAY_DEBUG  // Tuner display stuff


#endif	/* GUITAR_TUNER_H */

