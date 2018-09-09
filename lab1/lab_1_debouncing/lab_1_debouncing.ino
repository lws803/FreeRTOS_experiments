unsigned char flag=0;
unsigned long interrupt_time = millis();


void isr() {
    unsigned long curr_time = millis();
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    if (curr_time - interrupt_time > 200) {
        flag=!flag;
        interrupt_time = curr_time;
    }
}
void setup() {
    attachInterrupt(digitalPinToInterrupt(2), isr, RISING);
    pinMode(6, OUTPUT);
}
void loop() {
    digitalWrite(6, flag);
}
