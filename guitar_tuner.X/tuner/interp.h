/* 
 * File:   interp.h
 * Author: tahull
 * 
 */

#ifndef INTERP_H
#define	INTERP_H

#include <stdint.h>
#include <stdio.h>

#ifdef	__cplusplus
extern "C" {
#endif

//#define FIXED_POINT_INTP_SHIFT 5
    
/* @Summary
 *  Apply parabolic interpolation around three points
 * @Description
 *  Interpolation returns the difference from current center point to actual of
 *  center of the arc
 *  p = (1/2)((a-c)/(2b-c-a))
 * @Param
 *  int16_t alpha  : amplitude of leading point
 *  int16_t beta   : amplitude of center point
 *  int16_t gamma  : amplitude of trailing point
 *  uint8_t q      : fixed point shift
 * @Return
 *  uint16_t : return the amount to adjust
*/
int8_t interp(int16_t alpha, int16_t beta, int16_t gamma, uint8_t q);

#ifdef	__cplusplus
}
#endif

#endif	/* INTERP_H */

