unsigned char flag=0;
unsigned long interrupt_time = millis();


void isr() {
    unsigned long curr_time = millis();
    
    if (curr_time - interrupt_time > 200) {
        flag=!flag;
        interrupt_time = curr_time;
    }
}

void setup() {
    attachInterrupt(digitalPinToInterrupt(2), isr, RISING);
    pinMode(6, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    digitalWrite(6, flag);
}
