# Inter Task Communications

## Cooperating process

- Independent process cannot be affected or be affected by the execution of another process
- Cooperating process can affect or be affected by the execution of other process

## Shared memory

Processes/ tasks share information via a common buffer/ variable

### Pros
More efficient in passing messages

### Cons
Not scalable and does not support multi-machine

## Message queue

### Properties
- Messages are not shared among other processes. Process will copy the message from the queue over before popping it off.

- Ownership of message is passed from sender to receiver when the message is transferred. (if the sender modifies the message after sending, it will only modify local copy)

### Pros
Message passing can work on multiple machines or done locally

### Cons
- Complex queue required to send messages 
- May take up more memory in main memory


### Message passing schemes

#### Naming schemes
1. Direct naming scheme - Sender names the receiver by passing its task/ process ID to send()
2. Indirect naming scheme - messages are passed in a mailbox, channel or **message queue**

3. Symmetric naming scheme - Sender names either the receiver or the destination mailbox, receiver does vice versa
4. Asymetric naming scheme - Sender names either the receiver or the destination mailbox, receiver does not name the source of the message in any way. eg. Client-Server paradigm

### Synchronisation model

#### Blocking vs Non-blocking receive
- Blocking receive - Receiver must wait for message if it is not available

- Non-Blocking receive - Receiver mvoes on if message is not available


#### Synchronous message passing
- Receiver is **blocked** until it receives a message
- Sender is **blocked** until Receiver receives the message, after sending

Both **sender and receiver** are blocked and will wait for each other

#### Asynchronous message passing
**Sender is never blocked** even if the receiver has not received.

##### Pros
Generally good practice as it will not block processes (process got freedom to continue first eg. hanging the browser while waiting for a reply from web server)

##### Cons
1. Program may be complex to understand, programs don't run in sequence anymore
2. Finite buffer size means system is not truly async, may come to overflow. (eg. DDOS-ing of a web server)



## Message queues in FreeRTOS
- Symmetric and indirect naming scheme
- Asynchronous message passing with finite buffers, sender proceeds as soon as the message has been pushed to queue

If queue is *full*, sender tasks will be blocked and placed in waiting queue **(whereby highest priority tasks will unblock first)**
If queue is *empty*, receiver tasks will be placed in waiting list (whereby highest priority tasks will go first)


### Misc

```c
BaseType_t xHigherPriorityTaskWoken;
xQueueSendToBackFromISR(... , &xHigherPriorityTaskWoken) 
// Similar to xSemaphoreGiveFromISR, this will give us the opportunity to jump back to the task that was waiting for a message. 
// Regardless if there's a higher priority task being scheduled
```

