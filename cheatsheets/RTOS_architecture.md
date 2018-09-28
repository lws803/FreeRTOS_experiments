# Real time software architectures
### Tradeoffs 
1. Functionality
2. System cost and complexity
3. Response time 

## Simple polled loop
```cpp
while (true){
    if (packet_here) {
        process_data();
        packet_here = 0; // Reset the flag
    }
}
```
## Simple polled loop with delay
```cpp
while (true){
    if (packet_here) {
        delay(21); // Wait for awhile
        process_data();
        packet_here = 0; // Reset the flag
    }
}
```

## Round Robin
```cpp
void loop () {
    task_1();
    task_2();
    task_1(); // Can support different cycle rates, make task 1 run twice as often
    task_3();
    // and more..
}
```
Good for when each task runs for a short time  
No priority support  
Fragile - adding a new task could potentially make it undesirable

## Round Robin with interrupts
ISR sets a flag, main round robin loop checks for these flags. If not flagged just skip

```cpp
bool flagA = false, flagB = false;
void ISR_A () {
    flagA = true;
}

void ISR_B () {
    flagB = true;
}

void loop() {
    if (flagA) {
        // Do something, if not activated just skip 
    }
    if (flagB) {
        // Do something
    }
}
```

However, when eg. flagA is being processed, it can still take awhile for this data to actually be processed  
Priority is still not supported

## Function Queue Scheduling
Use a priority queue to store function calls
```cpp
void ISR_A () {
    queue.push(funcA);
}
void ISR_B () {
    queue.push(funcB);
}

void funcA () {
    // process
}
void funcB () {
    // process
}

// Multiple dependencies eg. depends on funcA
void funcC () {
    process();
    queue.push(funcA);
}

void loop() {
    while (!queue.empty()); // Idle
    funcptr = queue.front();
    queue.pop();
    funcptr(); // execute
}
```
Added complexity


