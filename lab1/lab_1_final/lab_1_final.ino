#define PIN_PTTM 0
#define PIN_TOUCH 1

int blink_rate = 1000;
int brightness = 0;
unsigned char flag = 1;

unsigned long interrupt_time_1 = millis();
unsigned long interrupt_time_2 = millis();

void isr_1() {
    unsigned long curr_time = millis();
    if (curr_time - interrupt_time_1 > 200) {
        flag = 0;
        interrupt_time_1 = curr_time;
        Serial.println("Disabled"); // Printing for debugging
    }
}
void isr_2() {
    unsigned long curr_time = millis();
    // Curr time is taken to measure against the previous time when the interrupt was called, 
    // if time different is too little, the interrupt will not be triggered. 
    if (curr_time - interrupt_time_2 > 200) {
        flag = 1;
        interrupt_time_2 = curr_time;
        Serial.println("Enabled"); // Printing for debugging
    }
}
void setup() {
    // initialize serial communications at 115200 bps:
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), isr_1, RISING);
    attachInterrupt(digitalPinToInterrupt(3), isr_2, RISING);
}
int remap_touch (int val) {
    // Remap to a value of 75 to 450
    int max = 1023, min = 0;
    double output = (double)(val - min)/ (max - min) * (450 - 75);
    return (int)output + 75;
}
int remap (int val) {
    // Remap to a value of 0 to 255
    double temp = val/1023.0;
    temp *= 255.0;
    return (int)temp;
}
void blink_led(unsigned pinnum) {
    digitalWrite(pinnum, HIGH); // Set digital I/O pin to a 1
    delay(blink_rate); // Delay
    digitalWrite(pinnum, LOW); // Set digital I/O pin to a 0
    delay(blink_rate); // Delay
}

void loop() {
    int val, touch;
    // read potentiometer's value
    val = analogRead(PIN_PTTM);
    
    touch = analogRead(PIN_TOUCH);
    
    if (flag) {
        // if flag is true, then we will blink the led with respect to remapped values
        blink_rate = remap_touch(touch);
        brightness = remap(val);
    }
    // Else they will not be changed and LED settings will remain the same
    analogWrite(6, brightness);
    blink_led (7);
    delay(200);
}
