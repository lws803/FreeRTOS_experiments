# Task management

Program: *Static* entity/ template (usually an executable file)  
Process: *Dynamic* entity (usually spawned from programs)  
A process can spawn many *threads*

- A stack is usually attached to a thread, when thread dies the stack is freed
- A heap is usually accessible by all threads within a process

## Common functions of process management 
1. Memory management 
2. Process control block 
3. Process state
4. Context switch 
5. Process creations and deletions 
6. Threads


## Process in Memory
When a process is spawned, **text** and **data** is fixed while **heap** is dynamic  
Each thread will have its own stack but threads in the same process will share the same *heap*, each process will get its own **heap**  
Stack can grow especially during recursion calls

## Process control block 
is a data structure in the operating system kernel containing the information needed to manage the scheduling of a particular process.  
Information stored: 
1. Process state
2. Process number
3. Program counter - to store the address of the next instruction
4. CPU registers - to store temporary variables
5. Memory management information (eg. memory pointers) - pointer to memory layout: heap, stack, data and text etc. 
6. Information regarding open files (list of open files)
7. Pointer - to store the location to next PCB in queue

- **Note:** If we store more data in the PCB we will need more space per PCB and ultimately we will have less PCBs 

2 memory regions: user space and kernel space, the segregation is for security reasons. To protect OS PCBs  

## Process state 
1. Created
2. Running
3. Blocked
4. Ready - waiting for the CPU to execute
5. Terminated

### Who decides who's states
**Admission control** enables the transition of a process from created to *ready* (admits a newly created process)  
**Scheduler** determines if a ready process can transfer to **running state**  

#### Special cases
##### Running --> ready  
Preemption - OS forces the running process to relinquish the processor involuntarily  
Yield - process itself ask OS to reconsider the scheduling decision and maybe hand over the processor to another process.  

##### Blocked --> ready
For communication between other processes eg. process A depends on process B's output. Process A will be blocked when it is waiting  
For IO events *ISR* determines if a blocked state can be moved to the ready state  

##### Running --> terminated
Voluntarily - Process has completed execution and is no longer needed in the system (eg .closure of a program)  
Involuntary - Process encounters an unrecoverable error  

### Reasons for process state transition
1. Voluntary action
2. Consequence of OS decision
3. Occurence of an event triggered by hardware. 

## Context switch 
Is used to make it *seem* like multiple processes can run on the same processor  
Uses **PCB** to store and reload states  
IDLE == Ready  
Context switching relies on *timer interrupts* where timer interrupts will allow OS to make a decision on whether or not to continue the process or switch it to another process (block).  
During context switching, the system does no useful work  

### Reasons for context switch
1. Interrupt within current process which causes scheduler to free up CPU and allocate it to another process.
2. Blocked state (waiting for IO/ Event) - only for pre-emptive OS.

### Pre-emptive vs non pre-emptive
Pre-emptive means that CPU can be taken away by running process at **any time** by OS.  
Non pre-emptive means that once the CPU has been allocated the process, the process will will keep the CPU until it realeases the CPU either by terminating or *requesting* for IO/ event wait. 

## Threads
Also another data structure, it is a **unit of execution** within a process  
A thread (lightweight process) contains: 
1. thread id
2. program counter - address of the next instruction in that task
3. register set (general purpose/ status registers)
4. stack space

A thread shares the following with other threads in the same process: 
1. code section 
2. data section 
3. OS resources - eg. address space (heap, where the global variables reside)

Process: Process control block (PCB)  
Thread: Thread control block (TCB)  

### Pros and cons of threading 
- Pros: Extremely efficient inter-task and task-OS communication due to **shared memory**
- Pros: Switching between threads can benefit from caching
- Cons: Tasks cannot be protected from each other
