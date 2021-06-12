
#include "acf.h"

/* autocorrelation function
 * something like y(k) = sum(abs(x(n)*x(n+k)))
 */
uint16_t acf(uint16_t len, samp_t *arr, uint16_t fs, uint8_t t_min, uint16_t t_max){
    int32_t temp_alpha;
    int16_t temp_beta,alpha,beta,gamma,max_val = 0;
    uint16_t f, period, len_k;
    int8_t period_adjust;

#ifdef ACF_VERBOSE
    //start print of correlation points
    printf("acf = [");
#endif
    //test a window range
    for(uint16_t k = t_min; k < t_max; k++){
        temp_beta = (int16_t)temp_alpha;
        temp_alpha = 0;
        len_k = len-k;
        for(uint16_t n = 0; n < (len_k); n++){
            temp_alpha += (int32_t)arr[n]*arr[n+k]; // sum up elements at k
        }
        temp_alpha = temp_alpha/(len); // decaying average
#ifdef ACF_VERBOSE
        printf("%ld,",temp_alpha); // print acf array element
#endif
        //find the max value in this sequence
        if(temp_beta > max_val){
            max_val = temp_beta;
            period = k-1;
            gamma = beta;
            beta = temp_beta;
            alpha = (int16_t)temp_alpha;
        }
    }
#ifdef ACF_VERBOSE
    printf("%ld]\n",temp_alpha); // print final acf array element
#endif 
    period_adjust = interp(alpha, beta, gamma, ACF_FP_SHFT);
    //printf("adj %i", period_adjust);
            
    // calc frequency relative to sampling frequency
    //period <<=ACF_FP_SHFT;
    //f = (uint16_t)((((int32_t)fs*10)<<ACF_FP_SHFT)/((int16_t)period+period_adjust));
    f = (uint16_t)((((int32_t)fs*10)<<ACF_FP_SHFT)/((int16_t)(period<<ACF_FP_SHFT) + period_adjust)); 
    
    return f;
}
