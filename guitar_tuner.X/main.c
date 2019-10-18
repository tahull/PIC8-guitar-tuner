/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16F1829
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

//guitar tuner config
#define FS 2000 //sample frequency set by timmer0 interrupt
#define SAMPLE_SIZE 256u  //size of signal sample array
#define PEAK_THRESH 0 //adc threshold to check for, before collecting samples
#define VBIAS 0 //voltage bias from amplifier circuit

//global vars
int16_t gia16_samples[SAMPLE_SIZE] = { 0 }; //samples from adc
enum ge_state {scan,collect,process,pause} ge_gt_state; //guitar signal states

//function prototypes
void print_array(uint16_t len, int16_t *arr);
int16_t ADC_10bit(void);
uint16_t amdf(uint16_t len, uint16_t fs, int16_t *arr); // take sample frequence and sample array, return frequency

/* Average magnitude difference function
 * auto correlation function that's easy on math operations, no multiply
 * something like y(k) = sum(abs(x(n)-x(n+k)))
 * TODO test result sum, and finish pitch/frequency 
 */
uint16_t amdf(uint16_t len, uint16_t fs, int16_t *arr){
    int16_t diff = 0;
    uint16_t sum = 0;
    
    for(uint16_t k = 0; k < len; k++){
        for(uint16_t n = 0; n < (len-k); n++){
            diff = arr[n]-arr[n+k];
            if (diff < 0)
                diff = -diff;
            sum = sum + diff; // sum up elements at k
            
        }
        printf("sum[%i] %i",k,sum);
    }
    return sum;
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
    if (ge_gt_state == scan && read_adc > PEAK_THRESH)
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
void print_array(uint16_t len, int16_t *arr){  
    printf("[");
    for(uint16_t i = 0; i < len; i++ )
        if(i == len-1) // last item. no comma
            printf("%i",arr[i]);
        else
            printf("%i,",arr[i]);
    printf("]");
}

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
        // Add your application code
        if(ge_gt_state == process){
            uint16_t res = amdf(SAMPLE_SIZE, FS, gia16_samples);
            ge_gt_state = pause;
            print_array(SAMPLE_SIZE, gia16_samples);
        }
    }
}
/**
 End of File
*/
