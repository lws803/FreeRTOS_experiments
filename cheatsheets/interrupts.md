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

Processor waste alot of cycles waiting for something to happen.  
Repeatedly reads the device register to check if an expected event has taken place.  


## Interrupt driven IO 
Interrupts transfer control to the ISR generally thru the interrupt vector, which contains the addresses of all the service routines.  
When interrupted,  
The OS preserves the address of execution of current task and dumps it in the process stack before dumping in the ISR.  
After ISR completes, it will pop off the previous address and continue from where it left off.  

### Handling multiple interrupts 
If 2 interrupts are triggered at the same time, the one of higher priority will go first.

### Nested interrupts
A higher priority interrupt can preamp the processing of lower priority interrupt

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

## Calculating processor cycle based questions
Always take reference from a common unit eg. 
1. Instructions - safest choice (cycles * n times of operation)
2. Cycles 
3. Time 

eg.  
if a computer can transfer 16bytes at once, 4MB takes 250k times of this.  
if the entire operation takes 400 cycles, we have 400 * 250k = 100M instructions  

**DMA can transfer one huge chunk each time for a given clock cycle**  

## Manipulating atmega registers 
```cpp
/*
    General rule:
    Set high = use |= ____1_____ 
    Set low =  use &= ____0_____
    For checking conditions: We usually use & operator 

    Bit-shifting:
    eg. x = 0xFA
    x_high = (x & 0xF0) >> 4 // & to remove the lower byte 
    x_low = (x & 0x0F) // & to remove the uppper byte
*/
void switchLED () {
    if (!turn) {
        PORTB &= 0b11101111;
    }else {
        // Switch LED at pin 12 on. Pin 12 is PB4
        PORTB |= 0b00010000; // Assuming this is active high pin, where logic 1 will turn it ON
    }
}

void toggleLED () {
    PORTB ^= 0b00010000;
}

void conditions() {
    // To check if an input pin is currently high or low
    if (PORTB & 0b00100000)
        // Pin is currently high 

    if (!(PORTB & 0b00100000))
        // Pin is currently low 
}

int main () {
    DDRB |= 0b00001000; // Set pin to output 
    DDRD &= 0b00100000; // Set pin to input, make sure we do this to set the pin to input mode

    // 0 = input, 1 = output
    
    PIND |= 0b00100000; // Will set the output pin to active high, meaning it will register when pulled down 
    // Note: For output or input even if its active high or low, we do not care. We will still set this according to
    // whether it is an input or output. 
    switchLED();
}
```

## Sample interrupt code
```cpp
void int0_ISR () {
    // Do whatever you want
}
void setup() {
    attachInterrupt(0, int0_ISR, RISING);
}
```