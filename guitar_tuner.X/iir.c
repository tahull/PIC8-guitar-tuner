#include "iir.h"

static int16_t a1 = -433, a2 = 210, b0 = 20, b1 = -22, b2 = 20; // fc = 370 hz

static inline int16_t iir_df1(int16_t x0){
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

static inline uint16_t zc(int16_t yn){
    static uint16_t avg_cnt, cnt, point, accum;
    static int16_t a,b;
    uint16_t avg = 0;    

    b = a;
    a = yn;
    //find some amount of crossings
    if(avg_cnt < 15){
        //count difference between zero crossings
        if(a >= 0 && b < 0){
            if(point > 0){
                accum += cnt - point;
                avg_cnt++;
            }
            //found a crossing point
            point = cnt; 
        }
        cnt++;        
    }
    //
    else{
        //shift up for better accuracy
        avg = (uint16_t)(((uint32_t)accum<<5)/avg_cnt);
#ifdef ZC_DEBUG
        printf("accum: %u crossings: %u avg: %u \n", accum, avg_cnt, avg);
#endif        
        avg_cnt = 0;
        accum = 0;
        cnt = 0;            
        point = 0;            
    }
    return avg;
}

//1) try to determine approximate frequency of raw signal harmonics and all
//2) set coefficients for filter, filter signal
//3) determine frequency of filtered (cleaned up) signal
uint16_t iir_process(int16_t raw_val){
    int16_t yn;
    uint16_t f = 0, avg = 0;
    
    //2 filter
    yn = iir_df1(raw_val);
    //3 find average period
    avg = zc(yn);
    
    //move decimal point to 1/10 place
    if(avg != 0){
        f = (uint16_t)((((uint32_t)FS*10)<<5)/avg);
    }
    return f;
}
