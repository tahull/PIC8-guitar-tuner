#include "amdf.h"

/* Average magnitude difference function
 * auto correlation function that's easy on math operations, no multiply
 * something like y(k) = sum(abs(x(n)-x(n+k)))
 * accuracy depends on sampling frequency and the target frequency
 * interpolation is used to approximate the actual peak/trough of 
 * the parabolic curve between sampling points
 */
uint16_t amdf(uint16_t len, samp_t *arr, uint16_t fs){
    uint16_t alpha = 0, beta = 0, gamma = 0, temp_beta, temp_alpha = 32765, temp_gamma; 
    int16_t diff = 0, period_adjust = 0;
    uint16_t f, period;
    
    uint16_t min_val = 65535;
    //test a window range
    for(uint16_t k = 20; k < 70; k++){
        temp_gamma = temp_beta;
        temp_beta = temp_alpha;
        temp_alpha = 0;
        uint16_t len_k = len-k;
        for(uint16_t n = 0; n < (len_k); n++){
            diff = (arr[n]-arr[n+k]); // difference of elements
            //printf("an[%i]=%i an+k[%i+%i]=%i diff=%i \n",n,arr[n],n,k,arr[n+k],diff);
            if (diff < 0)       //abs
                diff = -diff;
            temp_alpha += (uint16_t)diff; // sum up elements at k
        }
        temp_alpha = temp_alpha/(len_k); // average
        //find the lowest value in this sequence, set alpha, beta, gamma
        if(temp_beta < min_val){
            min_val = temp_beta;
            period = k-1;
            gamma = temp_gamma;
            beta = temp_beta;
            alpha = temp_alpha;        
        }
    }
        
    //parabolic peak interpolation. +/- amount to adjust current period
    period_adjust = interp(alpha, beta, gamma);//(((int32_t)alpha - gamma)<<FIXED_POINT_INTP_SHIFT)/((int16_t)(2*(2*beta - alpha - gamma)));
    // calc frequency relative to sampling frequency
    f = (uint16_t)((((uint32_t)fs*10)<<FIXED_POINT_INTP_SHIFT)/((period<<FIXED_POINT_INTP_SHIFT) + period_adjust)); 
#ifdef INTP_DEBUG
    uint16_t f_raw = (uint16_t)(((uint32_t)fs*10)/period);

    printf("T: %u, pre interpolated f: %u.%u\n", \
            period,(uint16_t)(f_raw/10),(uint16_t)(f_raw%10));
    
    printf("adj: .%i interpolated f %u.%u\n", period_adjust, \
            (uint16_t)(f/10),(uint16_t)(f%10));
#endif
    return f;
}

int16_t interp(uint16_t alpha, uint16_t beta, uint16_t gamma){
    int16_t intp = 0;
#ifdef INTP_DEBUG
    int16_t A,B;
    int32_t A_shift;
    A = (int16_t)alpha-(int16_t)gamma;
    A_shift = ((int32_t)A)<<FIXED_POINT_INTP_SHIFT;
    B = (int16_t)(2*(2*beta - alpha - gamma));
    intp = (int16_t)(A_shift/B);
    
    printf("alpha: %i beta: %i gamma: %i \n",alpha,beta,gamma);
    printf("A: %i A_shifted: %li B: %i A_shifted/B: %i \n",A,A_shift,B,intp);    
#else
    intp = (int16_t)((((int32_t)alpha - gamma)<<FIXED_POINT_INTP_SHIFT)/(2*(2*beta - alpha - gamma)));
#endif
    return intp;
}

