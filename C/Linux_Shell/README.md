# Linux Shell Implementation<br />
## Authored by Ron Amar<br />
<br />
==Description==<br />
This program gets lines of text from the user through the console and writes them in a new history file.<br />
As a sentence is inputted the program checks if it is a valid linux shell command and if it is it executes that command 
and counts how many words and are in that sentence.
once the user inputs the word "history" the program prints to the console all of the content in the history file so far.<br />
once the user inputs the word "done" the program prints the number of commands entered that session as well as how many words
were in those commands (only the valid commands count), then the program stops.<br />
(the program is space heavy friendly)<br />
<br />
**Build 1.3**<br />
added support for pipes ('|') - "commands chaining"<br />
added support for background processes ('&')<br />
added support for 'nohup' commands <br />
<br />
<br />
==main functions==<br />

1. run - this method receive a file name and runs in an infinite loop untill "done" is recived. it counts the words and chars of the inputted sentence and sends said sentence to the "writeHistory" function.
2. writeHistory - opens an append file and adds the given sentence to the end of the file. closes the file when done.
3. printHistory - opens a read file and prints the entire content of the given file name. closes the file when done.
4. getWordsNChars - counts the words and chars in a given line and *adds* the amount to the given integer pointers
5. toDoubleArr - takes a line of text and turns it to an array of words
6. freeArgv - free dynamic memory
7. runCommand - runs a comman in a child process and returns the childs exit status
8. getCleanWord - puts a clean version (no spaces and puts '\0' at the end) of [line] in [newLine] 
9. checkDone - check for 'done' command and executes its purpose if entered
10. checkHistory - check for 'history' command and executes its purpose if entered
11. checkLineCommand - check for '![num]' command and executes its purpose if entered 
12. checkPipe - checking for pipe 
13. runPipe - runs a command with pipe
14. countPipes - counts the number of pipes
15. checkBackground - checks for '&' and removes it from [line]
16. checkNohup - checks for 'nohup' command and manages the process if found 
17. checkEcho - checks if there is an echo command and updates to location of the following quotes
18. translate - translates a command line to the appropriate corresponding commands in case it contains '![num]' commands in it 

==Program Files== <br />
ex3.c - runs a 'linux shell' that implements all the available linux shell commands (except [cd]). <br />
 <br />
==How to compile?== <br />
compile: 	gcc ex3.c -o ex3 <br />
run: 	 	./ex3 <br />
 <br />
==Input:== <br />
no input <br />
 <br />
==Output:== <br />
history file <br />
nohup file <br />
console log <br />
