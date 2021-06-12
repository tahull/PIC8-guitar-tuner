
#include "interp.h"

int8_t interp(int16_t alpha, int16_t beta, int16_t gamma, uint8_t q){
    int8_t intp = 0;
#ifdef INTP_VERBOSE
    int16_t A,B;
    int32_t A_shift;
    A = (int16_t)alpha-(int16_t)gamma;
    A_shift = ((int32_t)A)<<q;
    B = (int16_t)(2*(2*beta - alpha - gamma));
    intp = (int16_t)(A_shift/B);
    
    printf("alpha: %i beta: %i gamma: %i \n",alpha,beta,gamma);
    printf("A: %i A_shifted: %li B: %i A_shifted/B: %i \n",A,A_shift,B,intp);    
#else
    intp = (int8_t)((((int32_t)alpha - gamma)<<q)/(2*(2*beta - alpha - gamma)));
#endif
    return intp;
}