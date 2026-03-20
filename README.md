# net.c

## Purpose
This project is a learning journal of sorts, it started before my Data Structures course Spring of 26, and as I learn more I update this project with what I learned in/out of class whenever I have the time.

## Update March 19th 2026
- Moved utility functions outside of net.c and into neth.c, and the prototype functions into neth.h as well as struct type and defines.
- Changed from singly linked list to dynamic array;
- Linked lists had caused memory fragmentation and cache misses because of the non-contiguous node allocation, while dynamic arrays are contiguous in memory, making iteration faster with better cache locality, and less per-element overhead.

## Compilation:
I like to use `gcc`:

```bash
gcc -o net net.c neth.c
```

## Basic usage: 
```bash
 ./net -a : List all connections regardless of state.
 ./net -t : List connections with "ESTABLISHED" state.
 ./net -l : List connections with "LISTENING" state.
```
## To be implemented:

 1) Better connection filtering and search by parsing dynamic command-line args. (optarg or getopt).
 2) Live monitoring of /proc/net/tcp that updates every 2-3 seconds instead of a static capture.
 3) DNS name resolution.
 4) Export to JSON or CSV.
 5) Maybe utilize eBPF in the future for faster captures. 
