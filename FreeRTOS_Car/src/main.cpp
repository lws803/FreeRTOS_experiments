#include <Arduino.h>
#include <Arduino_FreeRTOS.h> 
#include <task.h>
#include "semphr.h"
#include "queue.h"

SemaphoreHandle_t brakeBinarySemaphore;
SemaphoreHandle_t accelBinarySemaphore;
SemaphoreHandle_t speedMutex;
SemaphoreHandle_t currDistanceMutex;

unsigned long interrupt_time_1 = millis();
unsigned long interrupt_time_2 = millis();

QueueHandle_t redLEDQueue;
QueueHandle_t infoQueue;

struct display{
    int desiredSpeed ;
    int currentSpeed ;
    int currentDistance ;
};

display car = {0,0,0} ;

void speedInterpreter () {
    xSemaphoreTake (speedMutex, portMAX_DELAY);
    xSemaphoreTake(currDistanceMutex, portMAX_DELAY);

    int safeSpeed = 0;
    if (car.currentDistance <= 256) {
        safeSpeed = 0;
    } else if (car.currentDistance <= 512) {
        safeSpeed = 1;
    } else if (car.currentDistance <= 768) {
        safeSpeed = 2;
    } else {
        safeSpeed = 3;
    }

    if (safeSpeed < car.currentSpeed) {
        // turn on red LED
        xQueueSend(redLEDQueue, NULL, 1);
    }

    car.currentSpeed = min(safeSpeed, car.desiredSpeed);
    xQueueSend(infoQueue, &car, 0);

    switch (car.currentSpeed) {
        case 0:
        // Update LED here
        digitalWrite(5, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        break;
        case 1:
        digitalWrite(5, 1);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        break;
        case 2:
        digitalWrite(5, 1);
        digitalWrite(6, 1);
        digitalWrite(7, 0);
        break;
        case 3:
        digitalWrite(5, 1);
        digitalWrite(6, 1);
        digitalWrite(7, 1);
        break;
    }
    xSemaphoreGive(speedMutex);
    xSemaphoreGive(currDistanceMutex);
}

void redLEDTask (void *p) {
    const TickType_t xDelay = pdMS_TO_TICKS(1000); // 1 sec 
    while (1) {
        xQueueReceive(redLEDQueue, NULL, portMAX_DELAY);
        // Turn on LED
        digitalWrite(13, 1);
        vTaskDelay(xDelay);
        // Turn off LED
        digitalWrite(13, 0);
    }
}

void readDistanceTask (void *p) {
    while (1) {
        // Serial.println(analogRead(0));

        const TickType_t xPeriod = pdMS_TO_TICKS(500);
        TickType_t xLastWakeTime;
        xLastWakeTime = xTaskGetTickCount();
        // Max value = 1024
        xSemaphoreTake(currDistanceMutex, portMAX_DELAY);
        car.currentDistance = analogRead(0);
        xSemaphoreGive(currDistanceMutex);

        speedInterpreter();

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void brakeButtonTask (void *p) {
    while (1) {
        xSemaphoreTake (brakeBinarySemaphore, portMAX_DELAY);
        xSemaphoreTake(speedMutex, portMAX_DELAY);
        car.desiredSpeed = (car.desiredSpeed > 0) ? car.desiredSpeed - 1 : car.desiredSpeed ;
        xSemaphoreGive(speedMutex);
        speedInterpreter();
    }
}

void accelButtonTask (void *p) {
    while (1) {
        xSemaphoreTake (accelBinarySemaphore, portMAX_DELAY);
        xSemaphoreTake(speedMutex, portMAX_DELAY);
        car.desiredSpeed = (car.desiredSpeed < 3) ? car.desiredSpeed + 1 : car.desiredSpeed ;
        xSemaphoreGive(speedMutex);
        speedInterpreter();
    }
}

void brakeButtonISR () {
    unsigned long curr_time = millis();
    BaseType_t xHigherPriorityTaskWoken;
    // Debouncing
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;

        xSemaphoreGiveFromISR(brakeBinarySemaphore, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            taskYIELD();
        }
    }
}

void accelButtonISR () {
    unsigned long curr_time = millis();
    BaseType_t xHigherPriorityTaskWoken;
    // Debouncing
    if (curr_time - interrupt_time_2 > 200) {
        interrupt_time_2 = curr_time;
        xSemaphoreGiveFromISR(accelBinarySemaphore, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            taskYIELD();
        }
    }
}


void UARTTask (void *p) {
    display output = {0,0,0};

    while (1) {
        const TickType_t xPeriod = pdMS_TO_TICKS(1000);
        TickType_t xLastWakeTime;
        xLastWakeTime = xTaskGetTickCount();
        // Create another queue to read from this
        xQueueReceive(infoQueue, &output, 0);

        if (Serial) {
            // Print the info here from the buffer
            Serial.print("DS: ");
            Serial.print(output.desiredSpeed);
            Serial.print(" ");
            Serial.print("CS: ");
            Serial.print(output.currentSpeed);
            Serial.print(" ");
            Serial.print("CD: ");
            Serial.print(output.currentDistance);
            Serial.println();
        }
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}


void setup() {
    Serial.begin(9600);

    brakeBinarySemaphore = xSemaphoreCreateBinary();
    accelBinarySemaphore = xSemaphoreCreateBinary();
    speedMutex = xSemaphoreCreateMutex();
    currDistanceMutex = xSemaphoreCreateMutex();

    redLEDQueue = xQueueCreate(1, sizeof(int));
    infoQueue = xQueueCreate(1, 3*sizeof(int));

    attachInterrupt(digitalPinToInterrupt(2), accelButtonISR, RISING);
    attachInterrupt(digitalPinToInterrupt(3), brakeButtonISR, RISING);

    xTaskCreate(brakeButtonTask, "brake", 100, NULL, 3, NULL); 
    xTaskCreate(accelButtonTask, "accelerate", 100, NULL, 3, NULL);
    // Same priority as accelerate as we assume that user wont hit both at the same time
    xTaskCreate(redLEDTask, "redBrakeLED", 100, NULL, 1, NULL);
    xTaskCreate(readDistanceTask, "readDistance", 100, NULL, 4, NULL); // Highest priority
    xTaskCreate(UARTTask, "infoTask", 100, NULL, 2, NULL);

    Serial.println("Initialised");
    vTaskStartScheduler();
}

void loop() {
}
