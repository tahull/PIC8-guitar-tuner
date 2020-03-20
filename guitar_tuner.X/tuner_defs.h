/* 
 * File:   guitar_tuner.h
 * Author: tahull
 *
 */

#ifndef GUITAR_TUNER_H
#define	GUITAR_TUNER_H

//#include "amdf.h"

//guitar tuner config
#define FS 5000 //sample frequency set by timmer interrupt rate
#define SAMPLE_SIZE 128  //size of signal sample array

#define ADCBITS 10
//voltage bias from amplifier circuit in milli volts
#define VBIAS 1800    //1.8v
#define ADCVREF 4096    //fvr positive reference voltage 4.096v
// adc offset. ex for 10 bit
// 4.096v/(2^10) = 4mv per bit. 1.8v(bias from voltage divider on op amp)/.004v = 450
#define ADCOFFSET (VBIAS/(ADCVREF/(1<<ADCBITS)))
// adc threshold to check for, before collecting samples
// set as +50% of the offset
#define TRIGGER_LEVEL (ADCOFFSET + ADCOFFSET/2)//adc threshold to check for, before collecting samples

// frequency limits. expected range of 50 hz to 400 hz
#define F_MIN 50
#define F_MAX 400
#define T_MIN FS/F_MAX      //minimum period normalized to sample frequency
#define T_MAX FS/F_MIN      //maximum period normalized to sample frequency 

//DSP tyes, autocorrelation or IIR low pass filter
#define AMDF    0
#define IIR     1

#define TUNER_MODE IIR

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
#define PRINT_SIGNAL_DEBUG    // Raw ADC sample buffer
//#define PRINT_AMDF_DEBUG      // Processed amdf vals
//#define PRINT_TUNING_DEBUG    // Tuner display stuff


#endif	/* GUITAR_TUNER_H */

