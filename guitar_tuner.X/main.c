/* main for guitar tuner project
 * implements AMDF autocorrelation to find pitch/frequency
 * code in "MCC Generated Files" folder is generated by MCC code configurator and
 * subject to microchip license see mcc.h
 */

#include "mcc_generated_files/mcc.h"

//guitar tuner config
#define FS 5000 //sample frequency set by timmer0 interrupt
#define SAMPLE_SIZE 128u  //size of signal sample array
#define TRIGGER_LEVEL 200 //adc threshold to check for, before collecting samples
// 10 bit adc, adc fvr positive reference set to 4.096v
// 4.096v/(2^10) = 4mv per bit. 1.8v(bias from voltage divider on op amp)/.004v = 450
#define VBIAS 450 //voltage bias from amplifier circuit

//debug
//#define PRINT_DEBUG

//global vars
int16_t gia16_samples[SAMPLE_SIZE] = { 0 }; //samples from adc
enum ge_state {scan,collect,process,pause} ge_gt_state; //guitar signal states

//function prototypes
int16_t ADC_10bit(void);
uint16_t amdf(uint16_t len, uint16_t fs, int16_t *arr); // take sample frequence and sample array, return frequency
#ifdef PRINT_DEBUG
void print_array(uint16_t len, int16_t *arr);
#endif

/* Average magnitude difference function
 * auto correlation function that's easy on math operations, no multiply
 * something like y(k) = sum(abs(x(n)-x(n+k)))
 * accuracy depends on sampling frequency and the target frequency
 * ex. at fs = 5Khz
 * approximate accurracy
 * +/- .5 hz at 60-100hz
 * ~ +/- 3 hz at 300-400 hz
 */
uint16_t amdf(uint16_t len, uint16_t fs, int16_t *arr){
    int16_t diff = 0, p_adj = 0;
    uint16_t f, alpha = 0, beta = 0, gamma = 0, period, thresh = 65000;
    uint8_t state = 0;
    
#ifdef PRINT_DEBUG
    //print the original array
    print_array(SAMPLE_SIZE, gia16_samples);
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
            thresh = alpha/2; // set new threshold, ignore harmonics
            state = 1;
        }
        //Find the index of the first lowest point
        if(state == 1 && (beta < thresh) && (int16_t)(beta - alpha) < 0){
            period = k - 1;
#ifndef PRINT_DEBUG
            break;
        }
    }
    //parabolic peak interpolation. +/- amount to adjust current period
    p_adj = (((int32_t)alpha - gamma)*50L)/((int16_t)(2*beta - alpha - gamma));
#else
            printf("%u",alpha);
            break;
        }
        printf("%u,",alpha);
    }
    f = ((uint32_t)fs*10)/period;
     //parabolic peak interpolation. +/- amount to adjust current period
    p_adj = (((int32_t)alpha - gamma)*50L)/((int16_t)(2*beta - alpha - gamma));

    printf("]\nT: %u, pre interpolated f: %u.%u adj: .%i\n", \
            period,(uint16_t)(f/10),(uint16_t)(f%10),p_adj);
#endif
    // calc frequency relative to sampling frequency
    f = ((uint32_t)fs*1000L)/((period*100L) + p_adj);
    return f;
}

/* Timer interrupt for adc sample frequency     
 */
void TMR0_Interrupt(void){
    static uint16_t idx = 0;
    static int16_t read_adc = 0;
    
    read_adc = ADC_10bit();
    
    // found a loud signal, is if from a guitar?, does it matter?
    // TODO: test if it's worth additional testing, like checking for a second
    // peak and checking if the period falls in an expected range
    if (ge_gt_state == scan && read_adc > TRIGGER_LEVEL)
        ge_gt_state = collect;
    if (ge_gt_state == collect){
        if(idx < SAMPLE_SIZE){
            gia16_samples[idx] = read_adc - VBIAS;
            idx++;
        }
        else{
            idx = 0;
            ge_gt_state = process;
        }
    }
}

/* read adc, using this rather than mcc's ADC_GetConversion(adc_channel_t channel)
 * function. Since this project is using one adc channel 
 * and doesn't switch the channel on/off, it doesn't need acq delay
 */
int16_t ADC_10bit(void){
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE);
    // Conversion finished, return the result
    return ((ADRESH << 8) + ADRESL); 
}

/* Print the sample array
 */
#ifdef PRINT_DEBUG
void print_array(uint16_t len, int16_t *arr){
    printf("orig_signal = [");
    for(uint16_t i = 0; i < len; i++ )
        if(i == len-1) // last item. no comma
            printf("%i",arr[i]);
        else
            printf("%i,",arr[i]);
    printf("]\n");
}
#endif

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    TMR0_SetInterruptHandler(TMR0_Interrupt);
    ADC_SelectChannel(channel_AN11);
    
    while (1)
    {
        if(ge_gt_state == process){
            ge_gt_state = pause;
            INTERRUPT_GlobalInterruptDisable();
            uint16_t res = amdf(SAMPLE_SIZE, FS, gia16_samples);
            printf("freq: %u.%u\n",(uint16_t)(res/10),(uint16_t)(res%10));
        }
    }
}
/**
 End of File
*/
