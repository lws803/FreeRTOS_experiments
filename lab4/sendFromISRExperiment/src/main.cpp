#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include "semphr.h"
#include "queue.h"

#define STACK_SIZE    200

QueueHandle_t global_queue;
unsigned long interrupt_time_2 = millis();
SemaphoreHandle_t xSemaphore_0 ;

void serialPrint(void *p) {
    while (1) {
        xSemaphoreTake(xSemaphore_0, portMAX_DELAY) ;
        int taskNum = 0;
        int xTicksToWait = 0;
        // if (xQueueReceive(global_queue, &taskNum, xTicksToWait)) {
        //     if (Serial) {
        //         Serial.println(taskNum);
        //     }
        // }
        if (Serial) {
            Serial.println("HelloWorld");
        }

    } 
}

void lineBreakTask (void *p) {
    while (1) {
        Serial.println("========");
        vTaskDelay(100);
    }
}

void lineBreakTaskLow (void *p) {
    while (1) {
        Serial.println("*******");
        vTaskDelay(100);
    }
}

void int0ISR() {
    unsigned long curr_time = millis();
    BaseType_t xHigherPriorityTaskWoken;
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        int test = 100;
        int xTicksToWait = 0;
        // xQueueSend(global_queue, &test, xTicksToWait); // Without fromISR
        // xQueueSendFromISR(global_queue, &test, &xHigherPriorityTaskWoken);
        xSemaphoreGiveFromISR(xSemaphore_0, &xHigherPriorityTaskWoken) ;
        if (xHigherPriorityTaskWoken) {
            taskYIELD();
        }
        // xSemaphoreGive(xSemaphore_0); // Non ISR version
        // Serial.println("triggered");
    }
}


void setup() {
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), int0ISR, FALLING);
    global_queue = xQueueCreate(1, sizeof(int)); // With a queue size of 1, only one can populate it

    /* create two tasks one with higher priority than the other */ 
    xSemaphore_0 = xSemaphoreCreateBinary() ;
    xTaskCreate(lineBreakTaskLow, "lineBreakLow", STACK_SIZE, NULL, 2, NULL);

    xTaskCreate(serialPrint, "SerialPrint", STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(lineBreakTask, "lineBreak", STACK_SIZE, NULL, 3, NULL);
    /* start scheduler */
    vTaskStartScheduler();

}

void loop() {
}
