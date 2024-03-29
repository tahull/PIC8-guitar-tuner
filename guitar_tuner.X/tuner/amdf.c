#include "amdf.h"

/* Average magnitude difference function
 * auto correlation function that's easy on math operations, no multiply
 * something like y(k) = sum(abs(x(n)-x(n+k)))
 * accuracy depends on sampling frequency and the target frequency
 * interpolation is used to approximate the actual peak/trough of 
 * the parabolic curve between sampling points
 */
uint16_t amdf(uint16_t len, samp_t *arr, uint16_t fs, uint8_t t_min, uint16_t t_max){
    uint32_t temp_alpha = INT16_MAX;
    int16_t diff = 0, period_adjust = 0;
    uint16_t alpha = 0, beta = 0, gamma = 0, temp_beta;
    uint16_t f, period, len_k, min_val = UINT16_MAX;   

#ifdef AMDF_VERBOSE
    //start print of correlation points
    printf("amdf = [");
#endif
    //test a window range
    for(uint16_t k = t_min; k < t_max; k++){
        temp_beta = (uint16_t)temp_alpha;
        temp_alpha = 0;
        len_k = len-k;
        for(uint16_t n = 0; n < (len_k); n++){
            diff = (arr[n]-arr[n+k]); // difference of elements
            if (diff < 0)       //abs
                diff = -diff;
            temp_alpha += (uint32_t)diff; // sum up elements at k
        }
        temp_alpha = temp_alpha/(len_k); // average
#ifdef AMDF_VERBOSE
        printf("%u,",temp_alpha); // print amdf array element
#endif
        //find the lowest value in this sequence, set alpha, beta, gamma
        if(temp_beta < min_val){
            min_val = temp_beta;
            period = k-1;
            gamma = beta;
            beta = temp_beta;
            alpha = (uint16_t)temp_alpha;
        }
    }
#ifdef AMDF_VERBOSE
    printf("%u]\n",temp_alpha); // print final amdf array element
#endif
        
    //parabolic peak interpolation. +/- amount to adjust current period
    period_adjust = interp((int16_t)alpha, (int16_t)beta, (int16_t)gamma,AMDF_FP_SHFT);//(((int32_t)alpha - gamma)<<FIXED_POINT_INTP_SHIFT)/((int16_t)(2*(2*beta - alpha - gamma)));
    // calc frequency relative to sampling frequency
    f = (uint16_t)((((int32_t)fs*10)<<AMDF_FP_SHFT)/((int16_t)(period<<AMDF_FP_SHFT) + period_adjust)); 
#ifdef INTP_VERBOSE
    uint16_t f_raw = (uint16_t)(((uint32_t)fs*10)/period);

    printf("T: %u, pre interpolated f: %u.%u\n", \
            period,(f_raw/10),(f_raw%10));
    
    printf("adj: .%i interpolated f %u.%u\n", period_adjust, \
            (f/10),(f%10));
#endif
    return f;
}
