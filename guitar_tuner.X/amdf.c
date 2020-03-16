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
uint16_t amdf(uint16_t len, samp_buf_t *arr, uint16_t fs){
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
        }
        //Find first peak
        if(state == 0 && (int16_t)(alpha - beta) <= 0){
            thresh = beta/2; // set new threshold, low enough to ignore harmonics
            state = 1;
        }
        //Find the index of the first lowest point in valid range
        else if(k > T_MIN){
            if(state == 1 && (beta < thresh) && (int16_t)(beta - alpha) < 0){
                period = k - 1;
//do debug messages
#ifdef PRINT_AMDF_DEBUG
            printf("%u]\n",alpha); // print final amdf array element
#endif
                break;
            }
        }
        //couldn't find a period within valid range
        else if (k > T_MAX){
//do debug messages
#ifdef PRINT_AMDF_DEBUG
            // print final amdf array element of failed attempt
            printf("%u]\n Could not find a valid period within: %u to %u ",alpha,T_MIN,T_MAX); 
#endif
            break;
        }
#ifdef PRINT_AMDF_DEBUG
        printf("%u,",alpha); // print amdf array element
#endif
    }
    //parabolic peak interpolation. +/- amount to adjust current period
    period_adjust = interp(alpha, beta, gamma);//(((int32_t)alpha - gamma)<<FIXED_POINT_INTP_SHIFT)/((int16_t)(2*(2*beta - alpha - gamma)));
    // calc frequency relative to sampling frequency
    f = (((uint32_t)fs*10)<<FIXED_POINT_INTP_SHIFT)/((period<<FIXED_POINT_INTP_SHIFT) + period_adjust);        
#ifdef PRINT_AMDF_DEBUG
    uint16_t f_raw = ((uint32_t)fs*10)/period;

    printf("T: %u, pre interpolated f: %u.%u\n", \
            period,(uint16_t)(f_raw/10),(uint16_t)(f_raw%10));
    
    printf("adj: .%i interpolated f %u.%u\n", period_adjust, \
            (uint16_t)(f/10),(uint16_t)(f%10));
#endif
    return f;
}

int16_t interp(int16_t alpha, int16_t beta, int16_t gamma){
    int16_t intp = 0;
    intp = (((int32_t)alpha - gamma)<<FIXED_POINT_INTP_SHIFT)/((int16_t)(2*(2*beta - alpha - gamma)));
    return intp;
}

