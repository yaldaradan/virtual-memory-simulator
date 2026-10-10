# Virtual Memory Simulator

A C program that simulates demand paging with LRU page replacement. It reads a stream of memory accesses, translates each virtual address to a physical address, and reports page hits, page faults, and swap usage.

## How it works

- Each 32-bit virtual address is split into a page number and an offset.
- A page table tracks each page's frame and its valid, dirty, and swapped bits.
- When all frames are full, the least recently used page is evicted.
  - If the evicted page is clean, the fault counts as **minor**.
  - If the evicted page is dirty, it is written to swap space and the fault counts as **major**.

## Build

```bash
gcc -o a4 a4.c
```

## Run

```bash
./a4 <num_frames> <offset_bits> < sample_addr_requests.txt
```

Example with 64 frames and 4 KB pages (12 offset bits):

```bash
./a4 64 12 < sample_addr_requests.txt
```

Each input line is an operation (`r` for read, `w` for write) followed by a virtual address:

```
w 16807
r 1622650073
```

## Output

The program prints the physical address for each access, then a summary:

```
major faults = ..., minor faults = ..., hits = ..., pages stored in the swap space = ...
```
