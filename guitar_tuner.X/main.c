/* main for guitar tuner project
 * implements AMDF autocorrelation to find pitch/frequency
 * code in "MCC Generated Files" folder is generated by MCC code configurator and
 * subject to microchip license see mcc.h
 */

#include "mcc_generated_files/mcc.h"
#include "tuner_defs.h"
#include "tuner_display.h"
#include "amdf.h"
#include "ssd1306_oled.h"


//global vars
samp_t sample_buff[SAMPLE_SIZE] = { 0 }; //samples from adc
enum tuner {SCAN,COLLECT,PROCESS,PAUSE} tuner_state; //sampling states

//function prototypes
adc_t ADC_read(void);
#ifdef RAW_SIGNAL_DEBUG
void print_array(uint16_t len, samp_t *arr);
#endif

/* Timer interrupt for adc sample frequency     
 */
void TMR0_Interrupt(void){
    static uint16_t idx;
    static adc_t adc_val;
    
    adc_val = ADC_read();
    
    // found a loud signal, is if from a guitar?, does it matter?
    // TODO: test if it's worth additional testing, like checking for a second
    // peak and checking if the period falls in an expected range
    if (tuner_state == SCAN && adc_val > TRIGGER_LEVEL)
        tuner_state = COLLECT;
    if (tuner_state == COLLECT){
        if(idx < SAMPLE_SIZE){
            sample_buff[idx] = adc_val - ADCOFFSET;
            idx++;
        }
        else{
            idx = 0;
            tuner_state = PROCESS;
        }
    }
}

/* read adc, using this rather than mcc's ADC_GetConversion(adc_channel_t channel)
 * function. Since this project is using one adc channel 
 * and doesn't switch the channel on/off, it doesn't need acq delay
 * 
 * Allow selecting adc bit resolution 1-16, this mcu is up to 10 bit resolution
 * support for sub 3 bit and above 10 bit is here just for implementing in a 
 * generalized form although pointless.
 * 
 * Assumes ADC result is left justified
 */
adc_t ADC_read(void){
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
    // Wait for the conversion to finish. ~30uS in this wait loop
    while (ADCON0bits.GO_nDONE);
    // Conversion finished, return the result
#if ADCBITS > 8
    return (((uint16_t)(ADRESH << 8) + ADRESL)>>(16-ADCBITS));
#else
    return ADRESH >> (8-ADCBITS); 
#endif
}

/* Print the sample array
 */
#ifdef RAW_SIGNAL_DEBUG
void print_array(uint16_t len, samp_t *arr){
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
    
#ifdef RAW_SIGNAL_DEBUG
    printf("adc offset: %d adc trigger: %d \n", ADCOFFSET, TRIGGER_LEVEL);
#endif  
    
    
    ssd1306_init();
    
    while (1)
    {
        if(tuner_state == PROCESS){
            tuner_state = PAUSE;
            INTERRUPT_GlobalInterruptDisable();
            
#ifdef RAW_SIGNAL_DEBUG
            //print the original array
            print_array(SAMPLE_SIZE, sample_buff);
#endif
            
            uint16_t f = amdf(SAMPLE_SIZE, sample_buff, FS);
            //printf("freq: %u.%u\n",(uint16_t)(f/10),(uint16_t)(f%10));
            tuner_display(f);
            
            tuner_state = SCAN;
            INTERRUPT_GlobalInterruptEnable();
        }
    }
}
/**
 End of File
*/
