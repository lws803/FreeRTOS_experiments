# Memory allocation
Memory in the form of contiguous blocks

## Operations

1. Request (malloc call)
2. Free

## Memory allocation techniques

#### Variable allocation size (assign according to the declared variable)
Allocate exact size to the request amount

#### Fixed allocation sizes
There is a fixed allocation unit

## Fragmentation

### External fragmentation
Happens when **variable allocation** gets deallocated, a hole will be present.


### Internal fragmentation
Happens when memory is allocated but unused, happens during **fixed allocation**

### External vs internal fragmentation

- Externally fragmented memory can be compacted later
- Fixed allocation may lead to internal fragmentation, but less overhead to keep track of free memory

## Free list

Doubly linked list is used to keep track of holes. Pointers are built directly into the holes
  
Free lists are stored in the kernel space

- Note: It is more preferable to keep holes as large as possible (can satisfy small requests, less overhead in tracking memory, fewer holes)

#### Freeing memory

When memory is freed, holes are merged if neighbouring is a hole too

## Allocation algorithms

### Best fit

Pick smallest hole that will satisfy the request first

#### Pros and cons
- Have to search the entire list every time
- Tends to lead a lot of small holes

### Worst fit
Pick the largest hole to satisfy request

#### Pros and cons

- Have to search the entire list
- Still leads to fragmentation issues
- Usually worse than best fit


### First fit
Pick the largest hole that is large enough to satisfy the requirement

#### Pros and cons
- Much faster than worst or best fit
- Has fragmentation issues similar to best fit

### Next fit
Exactly like first fit but search from where it was left off

#### Pros and cons
- Usually faster than first fit

## Compaction
- Internal fragmentation: paging or segmentation
- External fragmentation: Can do compaction

Flush all allocated memory to one end, combine all holes to the other hand

### Problems
1. Tremendous overhead to copy all data
2. Must find and change all pointer values especially in the software

## Fixed size allocation: Bitmap

Represent each chunk or hole as a bit - can do bit masking to find out which is a hole and which is a chunk

### Pros and cons
Might reduce some overhead but generally still similar to normal method as we still have to iterate through

## Multiple free list
- Keep multiple list of different hole sizes
- Take hole from a list that most closely matches the size of request
- Leads to internal fragmentation

### Steps to populating a free list
1. Divide whole memory in 2
2. Take one side and check, the other will be added to the free list
3. If memory is still too big, repeat step 1 on current memory


## Buddy system
### Rules
- Each hole in a free list has a buddy
- If a hole and its buddy are combined, they create a larger new hole (twice as big)

### Steps: allocating memory 
1. Break a whole memory into 2
2. Take one and check if its still too big
3. If yes, perform step 1 on current block


### Steps: freeing memory
1. If a chunk just returned and its buddy is in the free list, merge and form new hole
2. Move up the level and repeat the same until cannot merge anymore