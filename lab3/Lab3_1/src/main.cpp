#include <Arduino.h> 
#include <avr/io.h> 
#include <Arduino_FreeRTOS.h> 
#include <task.h>

#define STACK_SIZE 200 
#define LED_PIN 6

void task1(void *p) {
    for (;;) { 
        digitalWrite(6, HIGH); 
        delay(1000); 
        digitalWrite(6, LOW); 
        delay(1000);
        } 
    }
void setup() {
       pinMode(LED_PIN, OUTPUT);
}
void loop() { 
    xTaskCreate( task1, "Task1", STACK_SIZE, NULL, 1, NULL);
// created task can be referenced.
// Pointer to the task entry function
// Task name
// Stack size
// Pointer that will be used as parameter // Task priority
// Used to pass back a handle by which the
    vTaskStartScheduler();
}