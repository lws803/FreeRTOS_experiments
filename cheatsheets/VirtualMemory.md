# Virtual Memory

Each process has its own private *virtual address space*, it can use the *physical memory* as a faster means of data processing by copying/ mapping data from virtual memory to physical memory.
  
CPU runs on virtual address not the main memory. It will talk to the MMU (memory management unit) then address the main memory.

## MMU
A physical hardware which manages the mapping and management of physical memory and virtual memory.

## Virtual memory pages

Virtual memory is partitioned into number of blocks called pages. These pages can range from 4kb to 4Mb large.

### Statuses of virtual memory pages
1. Unallocated - the page is not being used by the process
2. Uncached - The page is being used by process but it is not cached in physical memory
3. Cached - The page is being used by process and is cached in physical memory

Transfer between disk and physical memory is done via DMA

### Calculating virtual memory
Each bit corresponds to one byte.  
eg. 2^32 for a 32 bit system with 32 bit virtual address space = 4Gb (2^32 direct translation, no need to divide by 8)
##### Calculate bits needed for virtual/ physical address
```
virtual_address_space - offset_bits
physical_address_space - offset_bits

virtual_address_space_bits = 32 bit system
physical_address_space_bits = log(how many gb ram)/log(2)
```
##### Calculate num pages (virtual/ physical)
- number of pages: `num_bytes/page_size_in_bytes`
- number of bits required for the virtual page ID: `ln(number_of_pages)/ln(2)`

##### Calculate bits needed for page offset, how many bits to access an address within a page
```
ln(num_bytes_for_page)/ln(2)
```

##### What does the virtual/ physical address consist of
```
VPN + VPO
```
Virtual page number + virtual page offset. Offset is calculated from `ln(num_bytes_for_page)/ln(2)`

```
PPN + PPO
```
Physical page number + physical page offset. Num offset bits is the same for virtual and physical

##### Page table translation
We want to convert virtual address to physical address. Num of offset bits stay the same for both.

Steps:
1. Convert the virtual address to bits
2. Split them up into `virtual page number` section and offset section
3. Look up the table to find what the `virtual page number` corresponds to and its corresponding `physical address page number`
4. Convert `physical address page number` to bits
5. Append the bits in front of the offset (offset stays the same for both)


## Page Tables
Array of page tables entries (PTE) that maps virtual pages to physical pages. 

- 1 process will have one page table.
- Page tables are stored in the physical memory/ main memory (DRAM)
- Page table should have one page table entry corresponding to teach virtual page
- Key fields in each PTE (Valid bit, physical page number)

### Page table size
- Bit for page table entry: 1 valid bit + bits_needed_for_page_offset
- Number page table entries = num virtual pages
- Total page table size = number of page table entries * Bit for page table entry

### Address translation with page table

MMU will do the translation. 

##### Page hit
Reference to virtual memory byte that is in physical memory.

- Note: For steps refer to lecture notes

##### Page Fault

Reference to virtual memory that is not in physical memory

###### Handling page fault

1. User access a memory location
2. That page does not exist on physical memory but is currently on disk
3. Page fault raises an exception
4. OS then looks up the virtual memory for the address we need and then begin transfer to physical memory
5. Returns to faulting instruction when transfer is complete
6. Successful memory access on the second try

###### Handling page fault (extension)
- 3a. If physical memory is full  
3a1. find a victim or dirty page frame to replace using replacement policy  
3a2. Page handler will then send the victim page to virtual memory


###### Verbose version
1. The computer hardware traps to the kernel and program counter (PC) is saved on the stack. Current instruction state information is saved in CPU registers.

2. An assembly program is started to save the general registers and other volatile information to keep the OS from destroying it.

3. Operating system finds that a page fault has occurred and tries to find out which virtual page is needed. Some times hardware register contains this required information. If not, the operating system must retrieve PC, fetch instruction and find out what it was doing when the fault occurred.

4. Once virtual address caused page fault is known, system checks to see if address is valid and checks if there is no protection access problem.

5. If the virtual address is valid, the system checks to see if a page frame is free. If no frames are free, the page replacement algorithm is run to remove a page.

6. If frame selected is dirty, page is scheduled for transfer to disk, context switch takes place, fault process is suspended and another process is made to run until disk transfer is completed.

7. As soon as page frame is clean, operating system looks up disk address where needed page is, schedules disk operation to bring it in.

8. When disk interrupt indicates page has arrived, page tables are updated to reflect its position, and frame marked as being in normal state.

9. Faulting instruction is backed up to state it had when it began and PC is reset. Faulting is scheduled, operating system returns to routine that called it.

10. Assembly Routine reloads register and other state information, returns to user space to continue execution.


###### Largest overheads when handling page faults
1. Page handler looking up for the page in virtual memory due to page fault and disk operation to bring the page into physical memory
2. Looking for a **victim** in page table (one which is valid) to replace based on a replacement policy

## PTE - page table entry
Contains information such as valid bit and physical page number if the virtual page is stored in physical


## Speed up translation
- Problem: Needs to keep looking up PTE, at least twice (translation and actual memory request)
- Solution: Add another cache
- Translation lookaside buffer (TLB): Small hardware cache inside MMU which contains page table entries for small number of virtual pages (most recent accesses)
- Location: inside MMU

#### Additional info
- Searches inside the TLB are done via associative search (search in parallel), it also stores both virtual page number and physical page number
- TLB hit eliminates costly memory access for page table entry
- TLB miss incurs additional memory access for the PTE

## Virtual memory advantages

1. Solves some of the issues with memory allocation in physical memory
2. Virtual memory uses fixed page-size allocations -> internal fragmentation if the process is not using the entire page.
3. Virtual memory does not require contiguous allocation in the physical memory for a process -> No external fragmentation.

Having larger page sizes will result in less page faults but also more holes as the spaces are not utilised

## Multi-processing support
Having more processes who do not take up the whole main memory will still result in page faults. This is because each process will still be given a private virtual address space and the processes can map to physical memory at the same time (running concurrently)
