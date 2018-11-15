# Virtual Memory

Each process has its own *virtual memory*, it can use the *physical memory* as a faster means of data processing by copying/ mapping data from virtual memory to physical memory.
  
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

##### Calculate bits needed for virtual/ physical address
```
ln(num_bits)/ln(2)
```
##### Calculate num pages (virtual/ physical)
- number of pages: `num_bytes/page_size_in_bytes`
- number of bits required for the virtual page ID: `ln(number_of_pages)/ln(2)`

##### Calculate bits needed for page offset, how many bits to access an address within a page
```
ln(num_bytes_for_page)/ln(2)
```

##### Number of bits required in a 32bit/ 64bit OS
```
2^32 or 2^64
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

## Page Tables
Array of [age tables entries (PTE) that maps virtual pages to physical pages. 

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
4. OS Page handler will load the page into physical memory
5. Modify page table entry corresponding to the virtual page (if physical memory is full, find a victim to replace)
6. Returns to faulting instruction
7. Successful memory access on the second try


## Speed up translation
- Solution: Add another cache
- Translation lookaside buffer: Small hardware cache inside MMU which contains page table entries for small number of virtual pages (most recent accesses)

## Virtual memory advantages

1. Solves some of the issues with memory allocation in physical memory
2. Virtual memory uses fixed page-size allocations -> internal fragmentation if the process is not using the entire page.
3. Virtual memory does not require contiguous allocation in the physical memory for a process -> No external fragmentation.

Having larger page sizes will result in less page faults but also more holes as the spaces are not utilised
