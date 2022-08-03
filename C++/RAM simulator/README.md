# Simulated operating system memory management <br />
## Authored by Ron Amar <br />

==Description== <br />
This program simulates the way a basic operating system manages its memory.
utilizing a simple paging method.

==main functions== <br />
1. load - reads a cell from one of the processes in the memory
2. store - writes a piece of information on a cell from one of the processes in the memory
3. checkMemoryFull - checks if the page we want to SWAP out is not empty (Memory Full) 
		     and if it is switches it out to the swap file in case its dirty (one of its values was changed)
4. getExecPage - puts the needed exec page in the main memory
5. initPageTable - initializing a page table for a process

==Program Files== <br />
- main.cpp - the main file, runs the program
- sim_mem.h - header file of the sim_mem class with all the structs and function declaration
- sim_mem.cpp - the code of all the functions in the header file
- exec_file_1.txt - process 1
- exec_file_2.txt - process 2


==How to compile?== <br />
compile: 	g++ main.cpp sim_mem.cpp -o main <br />
run: 	 	./main <br />

==Input:== <br />
no input <br />

==Output:== <br />
swap file <br />
console log <br />
