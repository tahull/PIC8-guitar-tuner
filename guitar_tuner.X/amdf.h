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

#define FIXED_POINT_INTP_SHIFT 8 

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
uint16_t amdf(uint16_t len, samp_t *arr, uint16_t fs);


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
int16_t interp(uint16_t alpha, uint16_t beta, uint16_t gamma);




#endif	/* AMDF_H */

