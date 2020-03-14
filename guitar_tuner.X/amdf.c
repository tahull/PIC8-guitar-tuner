#include "amdf.h"

/* Average magnitude difference function
 * auto correlation function that's easy on math operations, no multiply
 * something like y(k) = sum(abs(x(n)-x(n+k)))
 * accuracy depends on sampling frequency and the target frequency
 * ex. at fs = 5Khz
 * approximate accuracy
 * +/- .5 hz at 60-100hz
 * ~ +/- 3 hz at 300-400 hz
 */
uint16_t amdf(uint16_t len, int16_t *arr, uint16_t fs){
    int16_t diff = 0, period_adjust = 0;
    uint16_t f, alpha = 0, beta = 0, gamma = 0, period, thresh = 65000;
    uint8_t state = 0;
    
#ifdef PRINT_AMDF_DEBUG
    //start print of correlation points
    printf("amdf = [");
#endif

    for(uint16_t k = 1; k < len; k++){
        gamma = beta;
        beta = alpha;
        alpha = 0;
        for(uint16_t n = 0; n < (len-k); n++){
            diff = (arr[n]-arr[n+k])>>4; // sums get too large divide to avoid overflow
            //printf("an[%i]=%i an+k[%i+%i]=%i diff=%i \n",n,arr[n],n,k,arr[n+k],diff);
            if (diff < 0)
                diff = -diff;
            alpha += (uint16_t)diff; // sum up elements at k
//            if (sum > thresh){
//                printf("problem %u", sum);
//                break;
//            }
        }
        //Find first peak
        if(state == 0 && (int16_t)(alpha - beta) <= 0){
            thresh = beta/2; // set new threshold, low enough to ignore harmonics
            state = 1;
        }
        //Find the index of the first lowest point
        else if(state == 1 && (beta < thresh) && (int16_t)(beta - alpha) < 0){
            period = k - 1;
//no debug
#ifndef PRINT_AMDF_DEBUG
            break;
        }
    }
    //parabolic peak interpolation. +/- amount to adjust current period
    period_adjust = (((int32_t)alpha - gamma)*50L)/((int16_t)(2*beta - alpha - gamma));
    // calc frequency relative to sampling frequency
    f = ((uint32_t)fs*1000L)/((period*100L) + period_adjust);
//do debug messages
#else
            printf("%u]\n",alpha); // print final amdf array element
            break;
        }
        printf("%u,",alpha); // print amdf array element
    }
    f = ((uint32_t)fs*10)/period;

    printf("T: %u, pre interpolated f: %u.%u\n", \
            period,(uint16_t)(f/10),(uint16_t)(f%10));
    
    //parabolic peak interpolation. +/- amount to adjust current period
    period_adjust = (((int32_t)alpha - gamma)*50L)/((int16_t)(2*beta - alpha - gamma));
    // calc frequency relative to sampling frequency
    f = ((uint32_t)fs*1000L)/((period*100L) + period_adjust);
    printf("adj: .%i interpolated f %u.%u\n", period_adjust, \
            (uint16_t)(f/10),(uint16_t)(f%10));
#endif
    return f;
}
