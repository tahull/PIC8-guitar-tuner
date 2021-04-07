/* 
 * File:   amdf.h
 * Author: tahull
 *
 */

#ifndef AMDF_H
#define	AMDF_H

#include <stdint.h>
#include <stdio.h>
#include "tuner_defs.h"

#define FIXED_POINT_INTP_SHIFT 5

#if T_MAX*(1<<FIXED_POINT_INTP_SHIFT) > INT16_MAX
#warning "fixed point shift or max period could be too large"
#endif

/* @Summary
 *  apply AMDF autocorrelation to a sample array
 * @Description
 *  Average Magnitude Difference Function
 *  y(k) = sum(abs(x(n)-x(n+k)))
 * @Param
 *  uint16_t len : length of sample array
 *  samp_buf_t *arr : pointer to sample array
 *  uint16_t fs : sample frequency
 * @Return
 *  uint16_t : return frequency with decimal shifted, ex frequency of 82.4 will be 824
*/
uint16_t amdf(uint16_t len, samp_t *arr, uint16_t fs,uint8_t min, uint16_t max);


/* @Summary
 *  apply parabolic interpolation around three points
 * @Description
 *  Interpolation returns the difference from current center point to actual of
 *  center of the arc
 *  p = (1/2)((a-c)/(2b-c-a))
 * @Param
 *  uint16_t alpha : y value of leading point
 *  uint16_t beta : y value of center point
 *  uint16_t gamma : y value of center last
 * @Return
 *  uint16_t : return the amount to adjust
*/
int16_t interp(int16_t alpha, int16_t beta, int16_t gamma);




#endif	/* AMDF_H */

