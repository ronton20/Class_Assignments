# Pattern Matching
Authored by Ron Ariel

## Description
This program finds set patterns inside a given text and notifies which patterns were found and where they were found (from index to index)

## Main functions
1. `pm_init` - 	initializing the data structure that stores the patterns
2. `pm_addstring` - adds a pattern to the data structure
3. `pm_makeFSM` - 	should be used after all the patterns were added to the data structure,
			finalizing the data structure by connecting all its states to their failure states.
4. `pm_fsm_search` - going over the given text and finding all the patterns within it, returns them in the form of a pm_match list.
5. `pm_destroy` -	destroying the data structure of the patterns. 

## Program Files
- `dbllist.h` - contains structs and declerations of functions of the doubly linked list.
- `dbllist.c` - the file containing the implementations of the functions.
- `pattern_matching.h` - contains structs and declarations of methods that are linked to the pattern matching algorithm.
- `pattern_matching.c` - containing the implementations of the functions.

## How to compile
assuming the main file is called `main.c`:
compile:
```bash
gcc main.c dbllist.c pattern_matching.c -o main
```
run:
```bash
./main
```

## Input
no input files

## Output
console log
