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


