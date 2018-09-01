
#define PIN_PTTM 0
#define PIN_TOUCH 1

int blink_rate = 1000;

void setup() {
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
}

int remap_touch (int val) {
  int max = 1023, min = 0;
  double output = (double)(val - min)/ (max - min) * (450 - 75);
  return (int)output + 75;
}

int remap (int val) {
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
  blink_rate = remap_touch(touch);
  val = remap(val);

  analogWrite(6, val);
  blink_led (7);

  Serial.print("Touch: ");
  Serial.println(blink_rate);
  
  delay(200);
}
