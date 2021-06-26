/* 
 * File:   amdf.h
 * Author: tahull
 *
 */

#ifndef AMDF_H
#define	AMDF_H

#include "interp.h"
#include "../tuner_defs.h"

#define AMDF_FP_SHFT 5

#if T_MAX*(1<<AMDF_FP_SHFT) > INT16_MAX
#warning "fixed point shift or max period could be too large"
#endif

/* @Summary
 *  Apply AMDF autocorrelation to a sample array
 * @Description
 *  Average Magnitude Difference Function with normalized result
 *  y(k) = (1/(N-k))*sum(abs(x(n)-x(n+k)))
 * @Param
 *  uint16_t len : length of sample array
 *  samp_buf_t *arr : pointer to sample array
 *  uint16_t fs : sample frequency
 *  uint8_t min : minimum period normalized to sampling frequency
 *  uint8_t max : maximum period normalized to sampling frequency
 * @Return
 *  uint16_t : return frequency with decimal shifted, ex frequency of 82.4 will be 824
*/
uint16_t amdf(uint16_t len, samp_t *arr, uint16_t fs,uint8_t min, uint16_t max);

#endif	/* AMDF_H */

