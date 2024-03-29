#include <Arduino.h>
#include <Arduino_FreeRTOS.h> 
#include <task.h>
#include "semphr.h"
#include "queue.h"
SemaphoreHandle_t xSemaphoreBinary;
SemaphoreHandle_t xSemaphoreCountingProducer;
SemaphoreHandle_t xMutex;
unsigned long interrupt_time_2 = millis();

int circularBuffer[4] = {0};
int index = 0;
int indexConsumer = 0;

void producerTask(void *p) {
    // Loop to make it blink 5 times
    while(1) {
        xSemaphoreTake(xSemaphoreBinary, portMAX_DELAY) ;
        // Stop if ISR not pressed
        xSemaphoreGive(xSemaphoreCountingProducer);
        // Protect the circular buffer
        xSemaphoreTake(xMutex,portMAX_DELAY);
        circularBuffer[index] = analogRead(0);
        xSemaphoreGive(xMutex);
        index++;
        index %= 4;
    }
}
void consumerTask(void *p) {
    // Loop to make it blink 5 times
    while(1) {
        xSemaphoreTake(xSemaphoreCountingProducer, portMAX_DELAY);
        TickType_t xLastWakeTime ;
        xLastWakeTime = xTaskGetTickCount();
        const TickType_t xPeriod= pdMS_TO_TICKS(5000);
        // Protect the circular buffer
        xSemaphoreTake(xMutex,portMAX_DELAY);
        Serial.println(circularBuffer[indexConsumer]);
        xSemaphoreGive(xMutex);
        indexConsumer++;
        indexConsumer %= 4;

        vTaskDelayUntil(&xLastWakeTime, xPeriod) ;
    }
}

void producerTaskISR() {
    unsigned long curr_time = millis();
    BaseType_t xHigherPriorityTaskWoken;
    // Curr time is taken to measure against the previous time when the interrupt was called,
    // if time different is too little, the interrupt will not be triggered.
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        xSemaphoreGiveFromISR(xSemaphoreBinary, &xHigherPriorityTaskWoken ) ;

        // if (xHigherPriorityTaskWoken) {
        //     // Immediately head back to task waiting for the semaphore
        //     taskYIELD();
        // }
    }
}

void setup() {
    Serial.begin(9600) ;
    xSemaphoreBinary = xSemaphoreCreateBinary() ;
    xSemaphoreCountingConsumer = xSemaphoreCreateCounting(4,3) ;
    xSemaphoreCountingProducer = xSemaphoreCreateCounting(4,0) ;
    xMutex = xSemaphoreCreateMutex();
    xTaskCreate(producerTask, "producer", 200, NULL, 1, NULL ) ;
    xTaskCreate(consumerTask, "consumer", 200, NULL, 1, NULL ) ;
    attachInterrupt(digitalPinToInterrupt(2), producerTaskISR, RISING);
    vTaskStartScheduler();
}

void loop() {
    // put your main code here, to run repeatedly:
}
