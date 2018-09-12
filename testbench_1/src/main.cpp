#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "task.h"


void task1 (void *pvParameters) {
    Serial.println("Hello world");
    // xTaskDelete(NULL);
    vTaskDelete(NULL);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop () {
	xTaskCreate(task1, "Task1", 256, NULL, 2, NULL);
    vTaskStartScheduler();
}
