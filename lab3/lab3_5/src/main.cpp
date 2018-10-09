#include <Arduino.h> 
#include <avr/io.h> 
#include <Arduino_FreeRTOS.h> 
#include <task.h>

#define STACK_SIZE 200 
#define TIMSK1 _SFR_MEM8(0x6F)

void myDelay(int ms) {
    for (int i = 0; i < ms; i++) {
        delayMicroseconds(1000);
              }
}

void execute ( int TASK_WCET, int pin) {
    int count = 0 ;
    while (count < TASK_WCET) {
        digitalWrite(pin, HIGH);
        myDelay(50);
        digitalWrite(pin, LOW);
        myDelay(50);
        count +=100;
    }
}
void task1(void *p) {

    while (1){
        TickType_t xLastWakeTime ;
        xLastWakeTime = xTaskGetTickCount();

        const TickType_t xPeriod1 = pdMS_TO_TICKS(1000);
        const TickType_t xPeriod2 = pdMS_TO_TICKS(1100);
        const TickType_t xPeriod3 = pdMS_TO_TICKS(1000);
        const TickType_t xPeriod4 = pdMS_TO_TICKS(1800);

        int TASK_WCET_1 = 1500 ;
        int TASK_WCET_2 = 2500 ;
        int TASK_WCET_3 = 1700 ;
        int TASK_WCET_4 = 700 ;
  
        execute (TASK_WCET_1, 6) ;
        execute (TASK_WCET_2, 7) ;
        xLastWakeTime = xTaskGetTickCount() ;
        vTaskDelayUntil(&xLastWakeTime, xPeriod1) ;
        // vTaskDelay(xPeriod1);
        execute (TASK_WCET_1, 6) ;
        execute (TASK_WCET_3, 8) ;
        execute (TASK_WCET_4, 9) ;
        xLastWakeTime = xTaskGetTickCount() ;
        vTaskDelayUntil(&xLastWakeTime, xPeriod2) ;  
        // vTaskDelay(xPeriod2);
        execute (TASK_WCET_1, 6) ;
        execute (TASK_WCET_2, 7) ;
        xLastWakeTime = xTaskGetTickCount() ;
        vTaskDelayUntil(&xLastWakeTime, xPeriod3) ;
        vTaskDelay(xPeriod3);
        execute (TASK_WCET_1, 6) ;
        execute (TASK_WCET_3, 8) ;
        xLastWakeTime = xTaskGetTickCount() ;
        vTaskDelayUntil(&xLastWakeTime, xPeriod4) ;
        // vTaskDelay(xPeriod4);
    }
}
void setup() {
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT); 
    pinMode(9, OUTPUT);               
    xTaskCreate( task1, "Task1", STACK_SIZE, NULL, 1, NULL);
        // xTaskCreate( task2, "Task2", STACK_SIZE, NULL, 2, NULL);
        // xTaskCreate( task3, "Task3", STACK_SIZE, NULL, 3, NULL); 
        // xTaskCreate( task4, "Task4", STACK_SIZE, NULL, 1, NULL);        
    vTaskStartScheduler();   
}

void loop() { 

}