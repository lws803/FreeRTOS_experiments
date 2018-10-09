#include <Arduino.h> 
#include <avr/io.h> 
#include <Arduino_FreeRTOS.h> 
#include <task.h>

#define STACK_SIZE 200 

void task1(void *p) {
    const TickType_t xPeriod = pdMS_TO_TICKS(750);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    for (;;) { 
        digitalWrite(6, HIGH);
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
        digitalWrite(6, LOW);
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    } 
}

void task2(void *p) {
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) { 
        digitalWrite(7, HIGH);
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
        digitalWrite(7, LOW);
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    } 

}


void task3(void *p) {
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(2000);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) { 
        digitalWrite(8, HIGH);
        vTaskDelayUntil( &xLastWakeTime, xPeriod);
        digitalWrite(8, LOW);
        vTaskDelayUntil( &xLastWakeTime, xPeriod);
    } 
}



void setup() {
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    Serial.begin(9600);
    // Higher number higher priority
    xTaskCreate( task1, "Task1", STACK_SIZE, NULL, 3, NULL);
    xTaskCreate( task2, "Task2", STACK_SIZE, NULL, 2, NULL);
    xTaskCreate( task3, "Task3", STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
}

void loop() { 
}