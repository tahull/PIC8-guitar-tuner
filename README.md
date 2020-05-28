# PIC8-guitar-tuner
A guitar tuner on 8-bit pic microcontroller

Implements guitar tuner testing different DSP techniques
- [x] AMDF average magnitude difference function with parabolic interpolation
  - pros: Does well with lower ADC resolution
  - cons: Uses buffer, takes up memory. Need a large enough buffer to capture enough points to see at least one period of a signal
- [ ] Autocorrelation (not implemented)
- [ ] IIR elliptic filter implemented with fixed point direct form I (In progress)
  - pros: IIR doesn't need a buffer, processing can be done in real-time between sample points
  - cons: Constrained microcontrollers have limit math support, software math takes longer, to keep processing real-time, the sample frequency may need to be reduced

project configurations  
- `sim` configuration  
  - SCL sim file for injecting values to ADC
  - f_to_mv.py for generating test data
- `pickit3` configuration  


ADC resolution can be set in tuner_defs.h  
Lower resolution is better for AMDF
- Avoid overflow
- Operation takes less time
- Same accuracy as using higher ADC resolution   

```c
#define ADCBITS 7
```
