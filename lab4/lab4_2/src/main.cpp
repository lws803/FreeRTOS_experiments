#include <Arduino.h>
#include <avr/io.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include "semphr.h"
#include "queue.h"

#define STACK_SIZE    200

QueueHandle_t global_queue;

void serialPrint(void *p) {
    while (1) {
        /*receive taskNum from message queue */
        int taskNum = 0;
        int xTicksToWait = 0;
        if (xQueueReceive(global_queue, &taskNum, xTicksToWait)) {
            if (Serial) {
                Serial.print("Task ");
                Serial.println(taskNum);
            }
        }
    } 
}

void task1and2(void *p) {
    while (1) {
        int taskNum = (int) p;
        /* send taskNum to message queue */
        int xTicksToWait = 1;
        xQueueSend(global_queue, &taskNum, xTicksToWait);
        vTaskDelay(1);
    } 
}


void setup() {
    Serial.begin(115200);
    global_queue = xQueueCreate(1, sizeof(int)); // With a queue size of 1, only one can populate it

    /* create two tasks one with higher priority than the other */ 
    xTaskCreate(task1and2, "Task1", STACK_SIZE, (void * ) 1, 2, NULL); 
    xTaskCreate(task1and2, "Task2", STACK_SIZE, (void * ) 2, 3, NULL);
    xTaskCreate(serialPrint, "SerialPrint", STACK_SIZE, NULL, 1, NULL);
    /* start scheduler */
    vTaskStartScheduler();

}

void loop() {
}
