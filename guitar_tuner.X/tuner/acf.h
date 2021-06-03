/* 
 * File:   acf.h
 * Author: tahull
 * Autocorrelation function
 */

#ifndef ACF_H
#define	ACF_H

#include "../tuner_defs.h"

#ifdef	__cplusplus
extern "C" {
#endif

/* @Summary
 *  Apply autocorrelation to a sample array
 * @Description
 *  Autocorrelation Function with normalized result
 *  y(k) = (1/(N-k))*sum(abs(x(n)*x(n+k)))
 * @Param
 *  uint16_t len : length of sample array
 *  samp_buf_t *arr : pointer to sample array
 *  uint16_t fs : sample frequency
 *  uint8_t min : minimum period normalized to sampling frequency
 *  uint8_t max : maximum period normalized to sampling frequency
 * @Return
 *  uint16_t : return frequency with decimal shifted, ex frequency of 82.4 will be 824
*/
uint16_t acf(uint16_t len, samp_t *arr, uint16_t fs,uint8_t min, uint16_t max);



#ifdef	__cplusplus
}
#endif

#endif	/* ACF_H */

