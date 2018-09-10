# Task management

Program: *Static* entity/ template (usually an executable file)  
Process: *Dynamic* entity (usually spawned from programs)  
A process can spawn many *threads*

## Common functions of process management 
1. Memory management 
2. Process control block 
3. Process state
4. Context switch 
5. Process creations and deletions 
6. Threads


## Process in Memory
When a process is spawned, *text* and *data* is fixed while *heap* is dynamic  
Each thread will have its own stack but threads in the same process will share the same *heap*

## Process control block 
is a data structure in the operating system kernel containing the information needed to manage the scheduling of a particular process.  
Information stored: 
1. Process state
2. Process number
3. Program counter 
4. CPU registers - to store temporary variables
5. Memory management information (eg. memory pointers) - pointer to memory layout: heap, stack, data and text etc. 
6. Information regarding open files (list of open files)
7. Pointer - to store the location to next PCB in queue


2 memory regions: user space and kernel space, the segregation is for security reasons. To protect OS PCBs  

## Process state 
1. Created
2. Running
3. Blocked
4. Ready - waiting for the CPU to execute
5. Terminated

### Who decides who's states
*Admission control* enables the transition of a process from created to *ready*    
*Scheduler* determines if a ready process can transfer to *running state*  

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
Uses *PCB* to store and reload states  
IDLE == Ready  
Context switching relies on *timer interrupts* where timer interrupts will allow OS to make a decision on whether or not to continue the process or switch it to another process (block).  
During context switching, the system does no useful work  