#include <Arduino.h> 
#include <avr/io.h> 
#include <Arduino_FreeRTOS.h> 
#include <task.h>
#include <queue.h>

#define STACK_SIZE 200

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;
TaskHandle_t TaskHandle_4;


void myDelay(int ms) {
    for (int i = 0; i < ms; i++) {
        delayMicroseconds(1000);
    }
}

void task1(void *p) {
    // For loop very impt for keeping the tasks alive
    for (;;) { 
        TickType_t xLastWakeTime;
        xLastWakeTime = xTaskGetTickCount();
        const TickType_t xPeriod = pdMS_TO_TICKS(5000);
        int count = 0;
        while (count < 1500) {
            digitalWrite(6, HIGH);
            myDelay(50);
            digitalWrite(6, LOW);
            myDelay(50);
            count +=100;
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void task2(void *p) {
    for (;;) {
        TickType_t xLastWakeTime;
        xLastWakeTime = xTaskGetTickCount();
        const TickType_t xPeriod = pdMS_TO_TICKS(10000);    
        int count = 0;
        while (count < 2500) {
            digitalWrite(7, HIGH);
            myDelay(50);
            digitalWrite(7, LOW);
            myDelay(50);
            count +=100;
        }
        Serial.print("task 2 ");
        Serial.print(xTaskGetTickCount());
        Serial.print(" ");
        Serial.println(count);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void task3(void *p) {
    for (;;) {
        TickType_t xLastWakeTime;
        xLastWakeTime = xTaskGetTickCount();
        const TickType_t xPeriod = pdMS_TO_TICKS(10000);    

        int count = 0;
        while (count < 1700) {
            digitalWrite(8, HIGH);
            myDelay(50);
            digitalWrite(8, LOW);
            myDelay(50);
            count +=100;
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

    }
}

void task4(void *p) {
    for (;;) {
        TickType_t xLastWakeTime;
        xLastWakeTime = xTaskGetTickCount();
        const TickType_t xPeriod = pdMS_TO_TICKS(20000);    
        int count = 0;
        while (count < 700) {
            digitalWrite(9, HIGH);
            myDelay(50);
            digitalWrite(9, LOW);
            myDelay(50);
            count +=100;
            Serial.println(count);
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}


void setup() {
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    Serial.begin(9600);

    // Higher number higher priority
    xTaskCreate( task1, "Task1", STACK_SIZE, NULL, 4, &TaskHandle_1);
    xTaskCreate( task2, "Task2", STACK_SIZE, NULL, 3, &TaskHandle_2);
    xTaskCreate( task3, "Task3", STACK_SIZE, NULL, 2, &TaskHandle_3); // Just change this, both LEDs will blink
    xTaskCreate( task4, "Task4", STACK_SIZE, NULL, 1, &TaskHandle_4);

    vTaskStartScheduler();

}

void loop() { 

}