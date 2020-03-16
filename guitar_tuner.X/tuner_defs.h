/* 
 * File:   guitar_tuner.h
 * Author: tahull
 *
 */

#ifndef GUITAR_TUNER_H
#define	GUITAR_TUNER_H

//#include "amdf.h"

//guitar tuner config
#define FS 5000 //sample frequency set by timmer0 interrupt
#define SAMPLE_SIZE 128u  //size of signal sample array

#define ADCBITS 10
//voltage bias from amplifier circuit
#define VBIAS 1800    //1.8v
#define ADCVREF 4096    //fvr positive reference voltage 4.096v
// adc offset. ex for 10 bit
// 4.096v/(2^10) = 4mv per bit. 1.8v(bias from voltage divider on op amp)/.004v = 450
#define ADCOFFSET (VBIAS/(ADCVREF/(1<<ADCBITS)))
//#define TRIGGER_LEVEL 0 //adc threshold to check for, before collecting samples
#define TRIGGER_LEVEL (ADCOFFSET + ADCOFFSET/2)//adc threshold to check for, before collecting samples

//ADC bit resolution 
#if ADCBITS > 8
typedef int16_t samp_buf_t;
typedef uint16_t adc_t;
#else
typedef uint8_t adc_t;
#if ADCBITS < 7
typedef int8_t samp_buf_t;
#else
typedef int16_t samp_buf_t;
#endif
#endif

//debug. uncomment to print debug info
//#define PRINT_DEBUG
//#define PRINT_AMDF_DEBUG
//#define PRINT_TUNING_DEBUG


#endif	/* GUITAR_TUNER_H */

