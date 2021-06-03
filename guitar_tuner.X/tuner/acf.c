
#include "acf.h"

/* autocorrelation function
 * something like y(k) = sum(abs(x(n)*x(n+k)))
 */
uint16_t acf(uint16_t len, samp_t *arr, uint16_t fs, uint8_t t_min, uint16_t t_max){
    int32_t temp_alpha,max_val = 0;
    uint16_t f, period, len_k;   

#ifdef ACF_VERBOSE
    //start print of correlation points
    printf("acf = [");
#endif
    //test a window range
    for(uint16_t k = t_min; k < t_max; k++){
        temp_alpha = 0;
        len_k = len-k;
        for(uint16_t n = 0; n < (len_k); n++){
            temp_alpha += (int32_t)arr[n]*arr[n+k]; // sum up elements at k
        }
        //temp_alpha = temp_alpha/(len_k); // normalize and average
#ifdef ACF_VERBOSE
        printf("%ld,",temp_alpha); // print acf array element
#endif
        //find the max value in this sequence
        if(temp_alpha > max_val){
            max_val = temp_alpha;
            period = k;
        }
    }
#ifdef ACF_VERBOSE
    printf("%ld]\n",temp_alpha); // print final acf array element
#endif 
    // calc frequency relative to sampling frequency
    f = (uint16_t)(((uint32_t)fs*10)/period);

    return f;
}
