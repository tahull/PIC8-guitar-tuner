#include "iir.h"

static int16_t a1 = -401, a2 = 199, b0 = 22, b1 = -14, b2 = 22; // fc = 370 hz
static const int16_t coeffs[] = {18,-33,18,     //b e2
                                 256,-491,239,  //a e2
                                 18,-32,18,     //b a2
                                 256,-483,234,  //a a2
                                 18,-30,18,     //b d3
                                 245,-471,227,  //a d3
                                 19,-27,19,     //b g3
                                 256,-455,220,  //a g3
                                 20,-22,20,     //b b3
                                 256,-433,210,  //a b3
                                 22,-14,22,     // e4
                                 256,-401,199};

void set_coeff(uint8_t string_id){
    uint8_t b_pos;
    uint8_t a_pos;
    if(string_id > 5)
        string_id = 5;
    b_pos = string_id*6;
    a_pos = string_id*6+3;

    b0 = coeffs[b_pos];
    b1 = coeffs[b_pos+1];
    b2 = coeffs[b_pos+2];
    a1 = coeffs[a_pos+1];
    a2 = coeffs[a_pos+2];

}

int16_t iir_df1(int16_t x0){
    static int16_t x1,x2,y1,y2;
    int32_t yn;
    
    yn = b0*x0 + b1*x1 + b2*x2 - a1*y1 - a2*y2;
    yn >>= 8;
    x2 = x1;
    x1 = x0;
    y2 = y1;
    y1 = (int16_t)yn;
    return (int16_t)yn;
}

//static inline uint16_t zc(int16_t yn){
//    static uint16_t avg_cnt, cnt, point, accum;
//    static int16_t a,b;
//    uint16_t avg = 0;
//
//    b = a;
//    a = yn;
//    //find some amount of crossings
//    if(avg_cnt < 15){
//        //count difference between zero crossings
//        if(a >= 0 && b < 0){
//            if(point > 0){
//                accum += cnt - point;
//                avg_cnt++;
//            }
//            //found a crossing point
//            point = cnt;
//        }
//        cnt++;
//    }
//    //
//    else{
//        //shift up for better accuracy
//        avg = (uint16_t)(((uint32_t)accum<<5)/avg_cnt);
//#ifdef ZC_DEBUG
//        printf("accum: %u crossings: %u avg: %u \n", accum, avg_cnt, avg);
//#endif
//        avg_cnt = 0;
//        accum = 0;
//        cnt = 0;
//        point = 0;
//    }
//    return avg;
//}

//1) try to determine approximate frequency of raw signal harmonics and all
//2) set coefficients for filter, filter signal
//3) determine frequency of filtered (cleaned up) signal
//uint16_t iir_process(int16_t raw_val){
//    int16_t yn;
//    uint16_t f = 0, avg = 0;
//
//    //2 filter
//    yn = iir_df1(raw_val);
//    //3 find average period
//    avg = zc(yn);
//
//    //move decimal point to 1/10 place
//    if(avg != 0){
//        f = (uint16_t)((((uint32_t)FS*10)<<5)/avg);
//    }
//    return f;
//}
