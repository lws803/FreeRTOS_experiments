# Deadlock

Deadlock occurs when a set of blocked processes each holding a resource and waiting to acquire a resource held by another process in the set.

## Resource allocation graph

If graph contains a cycle
- If only one instance per resource type => deadlock
- If several instances per resource type, possibility of deadlock

## Deadlock conditions

- Mutual exclusion: Only one process at a time can use a resource
- Hold and wait: A process holding at least one resource is waiting to acquire additional resources held by other processes.
- No preemptions: A **resource** can be released only voluntarily by the process holding it (semaphore locked)
- Circular wait: there exist a set of waiting processes such that...

## Methods to handle deadlocks
1. Ensure that the system will never enter a deadlock state (careful planning)
2. Allow the system to enter the deadlock state then recover
3. Ignore the problem

## Deadlock prevention

#### Prevent at least one of the deadlock conditions to happen: 
- Mutual exclusion: not required for sharable resources
- Hold and wait: Must guarantee that whenever a resource requests a resource, it does not hold any other resource. All for one. (only when all are available then execute)
- No preemption: If a process that is holding some resources requests another resource that cannot be immediately allocated to it, then it will release all the resources it is currently holding. The process will restart later
- Circular wait: Allow a max number of processes to be simultaneously fighting for resource


## Deadlock avoidance

Deadlock-avoidance algo **dynamically examines** the resource-allocation state to ensure that the system never goes into unsafe state.
- When a process requests for a resource, the algo must determine if giving it the resource will put it at an unsafe state.
- If safe the resource is granted, else wait.

**System is safe if there exists a safe sequence for all processes.**

## Banker's algo (see code)

A safe system can have Multiple safe sequences.

## Handling requests for resources

#### steps:
1. When a process requests for some resources, we pretend to allocate those resources first. (Requested resources != Need)
2. Then we run the safety algorithm again
3. If we cannot find a case where needi <= Work, system will be unsafe if the resources were allocated.

## Deadlock detection (also seen in banker's algo)

```cpp
    // Initialise
    vector<int> work = available; // This makes sure that work is initialised to what's currently available at this snapshot.
    vector<bool> Finish (numProcesses, false); // All false
    

    while (true) {
        // Find an i such that its not finished and need <= work
        // ***Most important step. If all tasks are finished, then system is definitely in safe state, else if some are unfinished, we check if need <= work for all resources.
        // If work == 0 for all resources, and no process is finished and all processes needs are more than work, then the system is in an unsafe state
        int chosen = -1;
        for (int i = 0; i < numProcesses; i++) {
            // Most important section in determining if a process can be completed given the current amount of resources available.
            if (lessThanEqualsTo(need[i], work) && Finish[i] == false) {
                chosen = i;
                break;
            }
        }
        
        // If there's a chosen one, it is not finished yet and we add on to the working resources for next iteration
        if (chosen != -1) {
            // ***This implicity ensures that work is only added (workPlusAllocation) if it is not a finished task and allocating the resource it needs it will ensure that it completes
            work = workPlusAllocation(work, allocation[chosen]);
            Finish[chosen] = true;
            processSequence.push(chosen);
        } else {
            // Check if all processes are finished
            int count = 0;
            for (auto each : Finish) {
                if (each == true) {
                    count++;
                }
            }
            if (count == numProcesses) {
                cout << "System is safe!" << endl;
                break;
            } else {
                cout << "System is unsafe!" << endl;
                break;
            }
        }
    }
```

Time complexity: O(M*N^2) where M is num resource types and N is number of processes
