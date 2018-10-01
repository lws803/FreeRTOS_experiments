#include <Arduino.h> 
#include <avr/io.h> 
#include <Arduino_FreeRTOS.h> 
#include <task.h>

#define STACK_SIZE 200 
#define LED_PIN 11

void task1(void *p) {
    for (;;) { 
            digitalWrite(LED_PIN, HIGH); 
            delay(1000); 
            digitalWrite(LED_PIN, LOW); 
            delay(1000);

        } 
    }
void setup() {
    pinMode(LED_PIN, OUTPUT);
    
    xTaskCreate( task1, "Task1", STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}

void loop() { 
}