# FreeRTOS code snippet 1 

FreeRTOS tasks consists of a suspended mode which can only be resumed by calling *API* ```vTaskResume()```


## FreeRTOS task creation

```C++
void task1 (void *pvParameters) {/*Do Something*/}

void loop () {
    xTaskCreate(task1, "Task1", 256, NULL, 2, NULL);
}
```
## FreeRTOS task deletion 

```C++
void task1 (void *pvParameters) {
    /*Do Something*/
    vTaskDelete(NULL); // Delete myself
}
void loop () {
    xTaskCreate(task1, "Task1", 256, NULL, 2, NULL);
    vTaskStartScheduler();
}
```
- Each task has a priority 
- CPU goes to highest-priority task that is ready 
- Tasks with the same priority gets the equal share 


**Note:** FreeRTOS allows multi threading on the same process
