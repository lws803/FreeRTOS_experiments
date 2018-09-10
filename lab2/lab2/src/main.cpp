#include <Arduino.h>
#include <stdlib.h>
#include <limits.h>
int fun1(int x, int y) {
    return x+y;
}
int fun2(int x, int y) { 
    return x*y;
}

void setup() {
    Serial.begin(9600);
}
// Declare the function pointer
int (*funcptr)(int, int);

void loop() {
    float turn=(float) rand() / INT_MAX;
    int result;
    if(turn>0.5)
        funcptr=fun1;
    else
    // Invoke the function
    result=funcptr(2,3);
    Serial.print("Computation result:");
    Serial.println(result);
    //200ms pause
    delay(200);
    funcptr=fun2;
}