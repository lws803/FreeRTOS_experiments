#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "task.h"


QueueHandle_t global_queue = 0;


void sender_task (void *p) {
  int i = 0;
  while (true) {  
        if (!xQueueSend(global_queue, &i, 100)) {
    //      Serial.println("Failed to send");
        }else {
            i++;
    //      Serial.print(i++);
    //      Serial.print(" ");
    //      Serial.println("Sent!");
        }
        
        vTaskDelay(300);    
    }
}

void receiver_task (void *p) {
  int i = 0;
  while (true) {
        if (xQueueReceive(global_queue, &i, 100)) {
            Serial.print("Received.. ");
            Serial.println(i);
        }else{
            Serial.println("Failed to receive");
        }
    }
}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    global_queue = xQueueCreate(3, sizeof(int));
    
    xTaskCreate (sender_task, (const char *) "sender_task", 1024, NULL, 1, NULL);
    xTaskCreate (receiver_task, (const char *) "receiver_task", 1024, NULL, 1, NULL);

    vTaskStartScheduler();
}

void loop() {
    // put your main code here, to run repeatedly:

}
