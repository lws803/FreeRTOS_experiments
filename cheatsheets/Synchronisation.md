# Task sync

## Race condition

Concurrent access to a shared data can lead to inconsistency. 
This is usually because of pre-emption by the OS on tasks that are writing or reading to the shared variable.


## Critical section

Section within code which modifies and accesses shared data. 

## Atomic lock

Atomic = non-interruptable (even from interrupts)  
The reason we use this instead of a just a flag buffer is because the flag buffer can be accessed by 2 threads in an OS leading to deadlock.

```C
bool TestAndSet (bool *target) {
    // Chunk of code here is atomic

    // If key is available then quickly set it to true to acquire it.
    if (target == false) {
        target = true;
    }

    return target;
}
```

### Use of test and set

```C
while (1) {
    while (TestAndSet(&lock)); // acquire lock, if lock not available then wait
    // critical section
    lock = false; // release lock
}
```


## Semaphores
example implementation:

```C
typedef struct {
    int value;
    queue<void *> L; // this is a queue which stores all processes waiting to enter their criticial sections
    // priority_queue<void *> process; // It could also be a priority queue depending on the implementation
} semaphore
```

### P(S) operation
Also known as take operation. Call this process to take the lock if available.
If not, wait and be put to blocked state after putting the process into the queue within semaphore.

```C
if (S.value > 0) {
    S.value--;
} else {
    block(); // Block the caller of P(S)
}
```
If lock is unavailable, it will block the **Caller** of P(S)

### V(S) operation

Also known as the give operation. Call this process to release the semaphore.
```C
if (S.L.empty()) {
    S.value++; // Will cap it at a max amount
} else {
    wakeup(P); // Wake up next in queue
}
```

If the semaphore process queue is empty, then increment the semaphore **only up to its max amount**.

### Mutex

Sempahore but only with a max value of 1.
- Allow only one task to access a critical region at any one time.

### Binary semaphores (Use for ensuring order)
```C
Semaphore_Handle_t xBinary; // initially intialised to 1 

// Two concurrent processes:
void task1() {
    // critical section
    V(xBinary);
}

void task2 () {
    P(xBinary);
    // Critical section
}

int setup () {
    xBinary = createSemaphore();
    P(xBinary) // initialise the semaphore to 1
}
```


`task1` will execute first since `task2` cannot execute as xMutex is not available from start. `task1` will give xMutex after it is done in critical section.


## Priority inversion
A situation when higher priority tasks get delayed by lower priority tasks because they have control over resources which the higher priority task needs.

1. Task 3 - 1
2. Task 2 - 2
3. Task 1 - 3

- Higher priority == higher number


1. Task 3 has lock, Task 1 needs lock too.
2. Task 3 get's preempted by Task 2 as Task 2 doesn't need lock
3. Task 1 will have no choice but to wait for Task 2 + Task 1 to complete before obtaining lock


### Solution - Priority inheritance

- When Low priority Task has the lock, it is temporarily raised to a high priority. For eg, Task 2 cannot pre-empt Task 1 anymore and allow Task 1 to complete.

- After the low priority process release the lock, its priority is set back to low priority.

## Misc

We use xSemaphoreGiveFromISR from ISR in FreeRTOS and call taskYIELD if xHigherPriorityTaskWoken is pdTRUE. This is so that the scheduler can jump straight to the **previous task waiting for the semaphore** instead of another higher priority task (if there's one).

```C
BaseType_t xHigherPriorityTaskWoken;
xSemaphoreGiveFromISR(xSemaphore_1, &xHigherPriorityTaskWoken);
if (xHigherPriorityTaskWoken) {
    taskYIELD();
}
```

