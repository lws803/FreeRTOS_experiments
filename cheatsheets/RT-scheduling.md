# Real-time scheduling

Algorithm which consists of a set of rules for allocating tasks to the processor

- Worst case behaviour is always when all tasks arrive at time = 0

### WCET, C
Worst case execution time is the max amount of time required to complete a task without any interference from other activities

### Deadline, D

- Timing constraints
- The maximum amount of time allowed between task instance being released.

### WCRT, R
Worst case response time is the max elapsed time between the release of a task instance and its completion

- C <= R

## Policies 

1. Fixed priority - priority does not change with time 
2. Dynamic priority - priority may change with time

## Hyper period
LCM of the task periods. Schedule repeats itself every hyper-period


## RMS schedulability analysis
1. Feasible schedule - if the schedule can meet all the timing constraints
2. Optimal scheduling algo - A scheduling algo that produces a feasible schedule for a task set if one exists

### Utilisation
Utilisation of a periodic task set is the fraction of processor time spent in the execution of the task set

- It is a necessary condition for any task to have a feasible schedule.
- U <= 1 is not a sufficient condition for a task set to have a feasible schedule under **fixed priority**

### Utilisation bound
Sufficient condition: A task set with n tasks is schedulable under fixed priorities if ```U <= n(2^(1/n) - 1)```

- U-bound is not a **necessary condition**, task set may still be schedulable if U-bound is not met.

### General outline
```(U <= 1) > (U-bound)```
- Anything about U-bound may or may not be schedulable under fixed priority settings

## Rate monotonic analysis
1. Critical instant: The instant at which the release of the task will produce the largest response time.
2. Critical instance: **the task instance** released at the critical instant

#### Critical instant analysis
WCRT of task T at its critical instant should not be longer than its deadline

### Necessary and sufficient condition
Steps to find if a task is schedulable (eg. taking task 3 as the target):
1. Find the time instances to consider (k values)
2. Use the smallest t found from step 1 to obtain `Wi(t) = sum( Ck * ceil(t/Pk) ) = w1`
3. Use `w1` to substitute to the next t `Wi(w1) = sum( Ck * ceil(w1/Pk) ) = w2`
4. So on and so forth until `Wi(t) <= t`

Note: Wi(t) is the time the task in question will finish by eg. if `Wi(t) == 300`, then the task will finish executing by t=300

## Earliest deadline first
- Priority of a task depends on the current deadline of the active task instance == **dynamic priority scheduling**
- Scheduling decision is only made when any new task instance is released (released from period)

### Optimality
- EDF is an optimal scheduling policy
- EDF can always produce a feasible schedule if U <= 1

### RMS vs EDF
1. RMS is simple but may not be optimal as it is **fixed priority**
2. EDF is complex especially when deciding on priorities in the PQ every time a new task is released
3. EDF also has better responsiveness of aperiodic activities (non periodic activities)
4. EDF allows full processor utilisation.

## Cyclic executive

Hard coded scheduling.

### Cycles to consider
1. Major cycle (hyper period) = LCM
2. Minor cycle = GCD

Within a major cycle, there are mulitple minor cycles

### Advantages of cyclic executive
1. Minimises pre-emption if constructed carefully
2. No need for actual tasks, only procedure calls
3. No schduling and context switching overhead

## Summary
- EDF will have more context switches than RMS 
- Cyclic executive will have less context switches compared to RMS 

