#include <Arduino.h>
#include <Arduino_FreeRTOS.h> 
#include <task.h>
#include "semphr.h"
#include "queue.h"

SemaphoreHandle_t xSemaphoreBinary; 
SemaphoreHandle_t xSemaphoreCountingProducer;
SemaphoreHandle_t xSemaphoreCountingConsumer;
SemaphoreHandle_t mutex;
unsigned long interrupt_time_2 = millis();

int circularBuffer[4] = {0};
int index = 0;
int indexConsumer = 0;

void producerTask(void *p) {    
    // Loop to make it blink 5 times
    while(1) {
        xSemaphoreGive(xSemaphoreCountingProducer);
        xSemaphoreTake(xSemaphoreCountingConsumer, portMAX_DELAY);
        xSemaphoreTake(xSemaphoreBinary, portMAX_DELAY) ;

        circularBuffer[index] = analogRead(0);
        index++;
        index %= 4;

        xSemaphoreGive(mutex);
        // index = (index==4) ? 0 : index ;

        // for (int i = 0; i < 5; i++) {
        //     digitalWrite(6, HIGH);
        //     delay(100);
        //     digitalWrite(6, LOW);
        //     delay(100);
        // }
        // xSemaphoreGive(xSemaphore);
    }
}
void consumerTask(void *p) {
    // Loop to make it blink 5 times 
    while(1) {
        // xSemaphoreTake(xSemaphoreCounting, portMAX_DELAY );
        xSemaphoreTake(mutex, portMAX_DELAY);
        xSemaphoreGive(xSemaphoreCountingConsumer);
        xSemaphoreTake(xSemaphoreCountingProducer, portMAX_DELAY);
        TickType_t xLastWakeTime ;
        xLastWakeTime = xTaskGetTickCount();
        const TickType_t xPeriod= pdMS_TO_TICKS(5000);

        Serial.println(circularBuffer[indexConsumer]);
        indexConsumer++;
        indexConsumer %= 4;

        vTaskDelayUntil(&xLastWakeTime, xPeriod) ;
        // xSemaphoreGive(xSemaphore);
    }
}

void producerTaskISR() {
    unsigned long curr_time = millis();
    BaseType_t xHigherPriorityTaskWoken = pdTRUE ;
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        xSemaphoreGiveFromISR(xSemaphoreBinary, &xHigherPriorityTaskWoken ) ;


        // Serial.println("0 activated");
        // Serial.println(analogRead(0));
    }
}

void setup() {
    Serial.begin(9600) ;
    xSemaphoreBinary = xSemaphoreCreateBinary() ;
    xSemaphoreCountingConsumer = xSemaphoreCreateCounting(4,0) ;
    xSemaphoreCountingProducer = xSemaphoreCreateCounting(4,0) ;
    mutex = xSemaphoreCreateMutex();
    xTaskCreate(producerTask, "producer", 200, NULL, 1, NULL ) ;
    xTaskCreate(consumerTask, "consumer", 200, NULL, 1, NULL ) ;    
    attachInterrupt(digitalPinToInterrupt(2), producerTaskISR, RISING);
    vTaskStartScheduler();
}

void loop() {
    // put your main code here, to run repeatedly:
}