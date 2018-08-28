# IO processing 

## Types of IO processing 
1. Polling 
2. Interrupt driven 
3. DMA (Direct memory access)

## Polling 
```cpp
void loop() {
    while(!done); // Main polling code
    process();
}
```

TODO: FILL UP

## Interrupt driven IO 

TODO: FILL UP 

### Interrupt latency
Interrupt response time = interrupt latency + processing time  
Interrupt response time should be shorter than time between interrupts  
How to reduce:  
1. Keep ISR code to the minimum
2. Processing time of higher priority interrupts may result to interrupt latency of lower priority ones
3. Split interrupt processing - eg. let ISR set a flag and then main loop to handle the processing (following a scheduler policy).

### Problem
When transferring data, interrupts will be triggered per byte  
CPU gets interrupted while doing tasks 

## DMA 
CPU is only interrupted when entire document is read  
Device controller transfers blocks of data from buffer storage directly to main memory without CPU intervention  
DMA controller with handle requests


## Sample interrupt code
```cpp
void int0_ISR () {
    // Do whatever you want
}
void setup() {
    attachInterrupt(0, int0_ISR, RISING);
}
```