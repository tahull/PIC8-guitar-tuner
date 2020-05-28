#include "tuner_display.h"

/* Given a frequency, compare with a tuning table, format and output.
 * output to UART for now.
 * TODO decide display medium and alternate tunings 
 */
void tuner_display(uint16_t f){
    uint8_t note_name[][3] = {"E2","A2","D3","G3","B3","E4"};
    uint16_t notes[] = {E2, A2, D3, G3, B3, E4};
    uint16_t trans[] = {680, 960, 1280, 1710, 2210, 2880, 3700};
    //uint16_t trans[] = {E2-(A2-E2)/2, E2+(A2-E2)/2, A2+(D3-A2)/2, D3+(G3-D3)/2, G3+(B3-G3)/2, B3+(E4-B3)/2, E4+(E4-B3)/2};
    
    //find the tuning range that the frequency 'f' falls into
    for(uint8_t i = 0; i < 6; i++)
        //if frequency is between lower_range and upper_range
        if(f >= trans[i] && f < trans[i+1]){
#ifdef TUNE_DISPLAY_DEBUG
            printf("found: %u.%u target: %u.%u between: %u to %u\n", \
                    (uint16_t)(f/10),(uint16_t)(f%10), \
                    (uint16_t)(notes[i]/10),(uint16_t)(notes[i]%10),\
                    (uint16_t)(trans[i]/10),(uint16_t)(trans[i+1]/10));
#endif
            //check if frequency 'f' is in an acceptable accuracy range for 'good tuning
            if(f >= notes[i] - (TUNING_ACCURACY+i) && f <= notes[i] + (TUNING_ACCURACY+i)){
                printf("---(%s)---\n",note_name[i]);
            }
            //pitch too high
            else if(f >= notes[i] + (TUNING_ACCURACY3+i)){
                printf("---(%s)<<<\n",note_name[i]);
            }
            else if(f >= notes[i] + (TUNING_ACCURACY2+i)){
                printf("---(%s)<<-\n",note_name[i]);
            }
            else if(f >= notes[i] + (TUNING_ACCURACY+i)){
                printf("---(%s)<--\n",note_name[i]);
            }
            //pitch too low
            else if(f <= notes[i] - (TUNING_ACCURACY3+i)){
                printf(">>>(%s)---\n",note_name[i]);
            }
            else if(f <= notes[i] - (TUNING_ACCURACY2+i)){
                printf("->>(%s)---\n",note_name[i]);
            }
            else if(f <= notes[i] - (TUNING_ACCURACY+i)){
                printf("-->(%s)---\n",note_name[i]);
            }
            break;
        }
}

