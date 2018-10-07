#include <Arduino.h> 
#include <avr/io.h> 
#include <Arduino_FreeRTOS.h> 
#include <task.h>

#define STACK_SIZE 200 

void task1(void *p) {
    const TickType_t xPeriod = pdMS_TO_TICKS(1500);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    for (;;) { 
        // Dump cyclic executive here
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    } 
}

void setup() {
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    Serial.begin(9600);
    // Higher number higher priority
    xTaskCreate( task1, "Task1", STACK_SIZE, NULL, 3, NULL);

    vTaskStartScheduler();
}

void loop() { 
}