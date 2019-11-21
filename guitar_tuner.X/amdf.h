/* 
 * File:   amdf.h
 * Author: tahull
 *
 */

#ifndef AMDF_H
#define	AMDF_H

#include <stdint.h>
#include <stdio.h>


/* @Summary
 *  apply AMDF autocorrelation to a sample array
 * @Description
 *  Average Magnitude Difference Function
 *  y(k) = sum(abs(x(n)-x(n+k)))
 * @Param
 *  uint16_t len : length of sample array
 *  int16_t *arr : pointer to sample array
 *  uint16_t fs : sample frequency
 * @Return
 *  uint16_t : return frequency with decimal shifted, ex frequency of 82.4 will be 824
*/
uint16_t amdf(uint16_t len, int16_t *arr, uint16_t fs);




#endif	/* AMDF_H */

