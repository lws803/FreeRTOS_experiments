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
