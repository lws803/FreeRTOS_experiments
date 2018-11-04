#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "task.h"

#define LONG_TIME 0xffff

unsigned long interrupt_time_1 = millis();
unsigned long interrupt_time_2 = millis();

SemaphoreHandle_t xSemaphore_0 ;
SemaphoreHandle_t xSemaphore_1 ;

void int0task(void *p) {    
    // Loop to make it blink 5 times
    while(1) {
        xSemaphoreTake(xSemaphore_0, portMAX_DELAY) ;
        for (int i = 0; i < 5; i++) {
            digitalWrite(6, HIGH);
            delay(100);
            digitalWrite(6, LOW);
            delay(100);
        }
        // xSemaphoreGive(xSemaphore);
    }

}
// Flashes LED at pin 7: 5 times at 1 Hz
void int1task(void *p) {
    // Loop to make it blink 5 times 
    while(1) {
        xSemaphoreTake(xSemaphore_1, LONG_TIME );
        for (int i = 0; i < 5; i++) {
            digitalWrite(7, HIGH);
            delay(500);
            digitalWrite(7, LOW);
            delay(500);
        }
        // xSemaphoreGive(xSemaphore);
    }
}
void int0ISR() {
    unsigned long curr_time = millis();
    BaseType_t xHigherPriorityTaskWoken;
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        xSemaphoreGiveFromISR(xSemaphore_0, &xHigherPriorityTaskWoken ) ;
        Serial.println("0 activated");
        if (xHigherPriorityTaskWoken) {
            // Give control back to the scheduler so that the scheduler can select the next higher priority task
            // If there is a higher priority task in the queue, it will yield the current task that was blocked by the ISR
            taskYIELD();
        }
    }
}
void int1ISR() {
    unsigned long curr_time = millis();
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    BaseType_t xHigherPriorityTaskWoken = pdTRUE ;

    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        Serial.println("1 activated");
        xSemaphoreGiveFromISR(xSemaphore_1, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            // Give control back to the scheduler so that the scheduler can select the next higher priority task
            // If there is a higher priority task in the queue, it will yield the current task that was blocked by the ISR
            taskYIELD();
        }
    }
}

void setup() {
    Serial.begin(9600);
    xSemaphore_0 = xSemaphoreCreateBinary() ;
    xSemaphore_1 = xSemaphoreCreateBinary() ;
    xTaskCreate(int0task, "int0", 200, NULL, 2, NULL ) ;
    xTaskCreate(int1task, "int1", 200, NULL, 1, NULL ) ;
    attachInterrupt(digitalPinToInterrupt(2), int0ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(3), int1ISR, RISING);
    vTaskStartScheduler();
}

void loop() {

}