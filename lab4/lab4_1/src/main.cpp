#include <Arduino.h>
#include <avr/io.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include "semphr.h"

#define STACK_SIZE    200

SemaphoreHandle_t xMutex;

void task1and2(void *p) {
    while (1) {
        xSemaphoreTake(xMutex, portMAX_DELAY);
        int taskNum = (int) p;
        Serial.print("Task ");
        Serial.println(taskNum);
        // taskYIELD();
        xSemaphoreGive(xMutex);
        vTaskDelay(1);
    } 
}
void setup() {
    xMutex = xSemaphoreCreateMutex();
    Serial.begin(115200);
}

void loop() {
    /* create two tasks one with higher priority than the other */ 
    xTaskCreate(task1and2, "Task1", STACK_SIZE, (void * ) 1, 1, NULL); 
    xTaskCreate(task1and2, "Task2", STACK_SIZE, (void * ) 2, 2, NULL); 
    // Adapted from: https://community.nxp.com/docs/DOC-331922
    /* start scheduler */
    vTaskStartScheduler();
}