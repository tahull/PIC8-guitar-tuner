/* 
 * File:   iir.h
 * Author: tahull
 *
 */

#ifndef IIR_H
#define	IIR_H

#include <stdint.h>
#include <stdio.h>
#include "tuner_defs.h"

static inline int16_t iir_df1(int16_t x0);
static inline uint16_t zc(int16_t yn);
uint16_t iir_process(int16_t raw_val);




#endif	/* IIR_H */

