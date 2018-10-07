#include <Arduino.h> 
#include <avr/io.h> 
#include <Arduino_FreeRTOS.h> 
#include <task.h>

#define STACK_SIZE 200 

void task1(void *p) {

    for (;;) {
        for (int i = 0; i <= 100; i+=5) {
            int onTime = 10*i/100;
            int offTime = 20 - onTime;
            digitalWrite(6, HIGH);
            delay(onTime);
            digitalWrite(6, LOW);
            delay(offTime);
        }

        for (int i = 100; i >= 0; i-=5) {
            int onTime = 10*i/100;
            int offTime = 20 - onTime;
            digitalWrite(6, HIGH);
            delay(onTime);
            digitalWrite(6, LOW);
            delay(offTime);
        }

    } 
}



void setup() {
    pinMode(6, OUTPUT);
    Serial.begin(9600);
    // Higher number higher priority
    xTaskCreate( task1, "Task1", STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}

void loop() { 
}