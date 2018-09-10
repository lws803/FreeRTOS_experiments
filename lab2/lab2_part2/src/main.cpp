#include <Arduino.h>
#include "prioq.h"

unsigned long interrupt_time_1 = millis();
unsigned long interrupt_time_2 = millis();

TPrioQueue *queue; // Make new queue
// Declares a new type called "funcptr"
typedef void (*funcptr)(void);
// Flashes LED at pin 6: 5 times a 5 Hz
void int0task() {
    // Loop to make it blink 5 times 
    for (int i = 0; i < 5; i++) {
        digitalWrite(6, HIGH);
        delay(100);
        digitalWrite(6, LOW);
        delay(100);
    }
}
// Flashes LED at pin 7: 5 times at 1 Hz
void int1task() {
    // Loop to make it blink 5 times 
    for (int i = 0; i < 5; i++) {
        digitalWrite(7, HIGH);
        delay(500);
        digitalWrite(7, LOW);
        delay(500);
    }
}
void int0ISR() {
    unsigned long curr_time = millis();
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        Serial.println("enq 0"); // Printing for debugging
        enq(queue, (void *)int0task, 9);
    }
}
void int1ISR() {
    unsigned long curr_time = millis();
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        Serial.println("enq 1"); // Printing for debugging
        enq(queue, (void *)int1task, 0);
    }
}
void setup() {
    queue=makeQueue();
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), int0ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(3), int1ISR, RISING);
}
// Dequeues and calls functions if the queue is not empty
void loop() {
    if(qlen(queue)>0) {
		// Dequeue it
        funcptr function;
		function = (funcptr) deq(queue);
        function();
        Serial.println("deq-ed");
	}	
}
