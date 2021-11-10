/* main for guitar tuner project
 * implements AMDF autocorrelation to find pitch/frequency
 * code in "MCC Generated Files" folder is generated by MCC code configurator and
 * subject to microchip license see mcc.h
 */

#include "mcc_generated_files/mcc.h"
#include "tuner_defs.h"
#include "tuner_display.h"
//#include "tuner/amdf.h"
#include "tuner/acf.h"


//function prototypes
adc_result_t ADC_read(void);
#ifdef RAW_SIGNAL_VERBOSE
void print_array(uint16_t len, samp_t *arr);
#endif

/* Timer interrupt for adc sample frequency     
 */
void TMR0_Interrupt(void){
    static uint16_t idx;
    static uint16_t btn_delay_count;
    samp_t adc_val;
    
    //check button every x ticks. if button is pressed, change string select
    if (btn_delay_count >= BTN_DELAY){
        btn_delay_count = 0;
        if(IO_RC7_PORT == 0){
            btn_sel++;
            if(btn_sel >= 7)
                btn_sel = 0;                
            tuner_state = SELECT_MODE;
        }           
    } 
    btn_delay_count++;
    
    adc_val = (samp_t)ADC_read();
    
    // Wait for an initial signal amplitude before collecting samples
    if (tuner_state == SCAN && adc_val > TRIGGER_LEVEL)
        tuner_state = COLLECT;
    if (tuner_state == COLLECT){
        if(idx < SAMPLE_SIZE){
            adc_val = (samp_t)(adc_val - ADCOFFSET);//remove calculated dc offset
            //save new adc sample
            sample_buff[idx] = adc_val;
            idx++;
        }
        else{//process valid signal
            tuner_state = PROCESS;
        }
    }
    if (tuner_state == RESET){
        idx = 0;
        tuner_state = SCAN;
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
adc_result_t ADC_read(void){
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
    // Wait for the conversion to finish. ~30uS in this wait loop
    while (ADCON0bits.GO_nDONE);
    // Conversion finished, return the result
#if ADCBITS > 8
    return (((adc_result_t)(ADRESH << 8) + ADRESL)>>(16-ADCBITS));
#else
    return ADRESH >> (8-ADCBITS); 
#endif
}

/* Print the sample array
 */
#ifdef RAW_SIGNAL_VERBOSE
void print_array(uint16_t len, samp_t *arr){
    printf("raw_signal = [");
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
    static uint8_t t_min = T_MIN, t_max = T_MAX;
    const uint8_t t_bounds[]={T_MAX,FS/96,FS/128,FS/171,FS/221,FS/280,T_MIN};
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
    
#ifdef RAW_SIGNAL_VERBOSE
    printf("adc offset: %d adc trigger: %d \n", ADCOFFSET, TRIGGER_LEVEL);
#endif
    
    ssd1306_init();
    tuner_state = SELECT_MODE;

    while (1)
    {
        if(tuner_state == SELECT_MODE){
            INTERRUPT_GlobalInterruptDisable();
            if(btn_sel < 6){
                t_max = t_bounds[btn_sel];
                t_min = t_bounds[btn_sel+1];
            }
            else{
                t_max = T_MAX;
                t_min = T_MIN;
            }
            tuner_display_mode(btn_sel);
            tuner_state = RESET;
            INTERRUPT_GlobalInterruptEnable();
        }
        // process signal, pause interrupt, display frequency
        // then reset sample collecting
        if(tuner_state == PROCESS){            
            INTERRUPT_GlobalInterruptDisable();
            //uint16_t f = amdf(SAMPLE_SIZE, sample_buff, FS,t_min,t_max);
            uint16_t f = acf(SAMPLE_SIZE, sample_buff, FS,t_min,t_max);
#ifdef RAW_SIGNAL_VERBOSE
            //print the raw signal array
            //printf("freq: %u.%u\n",(uint16_t)(f/10),(uint16_t)(f%10));
            const uint16_t strings[6] = {82,110,146,197,246,329};
            printf("FS=%u\n", FS);
            if(btn_sel == 6)
                printf("f=auto\n");
            else
                printf("f=%u\n",strings[btn_sel]);
            print_array(SAMPLE_SIZE, sample_buff);
#endif
            tuner_display(f);            
            tuner_state = RESET;
            INTERRUPT_GlobalInterruptEnable();
        }
    }
}
/**
 End of File
*/
